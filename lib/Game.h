#include <algorithm>
#include "Character.h"

class Game {
private:
    int n_players;
    int werewolfCount;              // Number of werewolves
    int villagerCount;              // Number of villagers
    vector<Character *> characters; // Stores the characters of the players
    unordered_map<int, Character *> idToCharacter;
    vector<int> playersId;

    void initializeCharacters() {
        // Shuffle the playersId vector
        srand(unsigned(time(NULL)));
        random_shuffle(playersId.begin(), playersId.end());
        int playerIdIndex = playersId.size() - 1;

        // Add the characters based on the number of players
        int numWerewolves = n_players / 3; // Assuming 1/3 of players are werewolves
        werewolfCount = numWerewolves;
        villagerCount = n_players - numWerewolves;
        int numSeers = 1; // Assuming there is only one seer
        int numVillagers = n_players - numWerewolves - numSeers;
        for (int i = 0; i < numWerewolves; i++) {
            Character* temp = new Werewolf();
            temp->sd = playersId[playerIdIndex--];
            characters.push_back(temp);
            idToCharacter[temp->sd] = temp;
        }

        for (int i = 0; i < numSeers; i++) {
            Character* temp = new Seer();
            temp->sd = playersId[playerIdIndex--];
            characters.push_back(temp);
            idToCharacter[temp->sd] = temp;
        }

        for (int i = 0; i < numVillagers; i++) {
            Character* temp = new Villager();
            temp->sd = playersId[playerIdIndex--];
            characters.push_back(temp);
            idToCharacter[temp->sd] = temp;
        }
    }

public:
    Game(vector<int> playersId) : n_players(playersId.size()), playersId(playersId) {
        // Initialize the characters based on the number of players
        initializeCharacters();
    }

    void status() {
        updateCharactersCount();
        string status = "\n------------------------------\n";
        status += to_string(werewolfCount) + " werewolf " + to_string(villagerCount) + " villager\n";
        status += "------------------------------\n";
        status += "Index\tCharacter\tStatus\n";
        int count = 0;
        for (auto &character: characters) {
            string characterStatus = (character->isAlive) ? "Alive" : "Dead";
            string row = to_string(count++) + "\t" + character->getName() + "\t" + characterStatus + "\n";
            status += row;
        }
        status += "------------------------------\n";
        string customizedStatus;
        for (int i = 0; i < characters.size(); i++) {
            customizedStatus = status;
            customizedStatus += "Your index: " + to_string(i) + "\n";
            customizedStatus += "------------------------------\n";
            write(characters[i]->sd, customizedStatus.c_str(), customizedStatus.length());
        }
    }

    void promptNight() {
        for (auto &character: characters) {
            if (!character->isAlive)
                continue;

            if (character->nightActionParametersCount == 0) {
                character->nightAction();
            } else if (character->nightActionParametersCount == 1) {
                string response =
                        "You are " + character->getName() + "\nEnter the target\'s index for night action: ";
                write(character->sd, response.c_str(), response.length());
//                cout << "You are " << character.second->getName() << endl;
//                cout << "Enter the target's index for night action: ";
//                int targetIndex;
//                cin >> targetIndex;
//                character.second->nightAction(characters[targetIndex].second);
            } else {
                string response = "Something wrong!\n";
                write(character->sd, response.c_str(), response.length());
            }
        }
    }

    void promptDay() {
        for (auto &character: characters) {
            if (!character->isAlive)
                continue;

            if (character->dayActionParametersCount == 0) {
                character->dayAction();
            } else if (character->dayActionParametersCount == 1) {
                string response =
                        "You are " + character->getName() + "\nEnter the target\'s index for day action: ";
                write(character->sd, response.c_str(), response.length());
//                cout << "You are " << character.second->getName() << endl;
//                cout << "Enter the target's index for day action: ";
//                int targetIndex;
//                cin >> targetIndex;
//                character.second->dayAction(characters[targetIndex].second);
            } else {
                string response = "Something wrong!\n";
                write(character->sd, response.c_str(), response.length());
            }
        }
    }

    void promptVote() {
        for (auto &character: characters) {
            if (!character->isAlive)
                continue;

            string response = "Enter the target's index for voting: ";
            write(character->sd, response.c_str(), response.length());
//            int targetIndex;
//            cin >> targetIndex;
//            character.second->vote(characters[targetIndex].second);
        }
    }

    void processVote() {
        int index = 0;
        int maxVote = characters[0]->voteCount;
        bool maxVoteUnique = true;
        for (int i = 1; i < n_players; i++) {
            if (characters[i]->voteCount > maxVote) {
                maxVote = characters[i]->voteCount;
                index = i;
                maxVoteUnique = true;
            } else if (characters[i]->voteCount == maxVote) {
                maxVoteUnique = false;
            }
        }
        if (maxVoteUnique) {
            characters[index]->isAlive = false;
            string response = "Player index " + to_string(index) + " is hung by villagers!";
            write(characters[index]->sd, response.c_str(), response.length());
        } else {
            string response = "Villagers couldn't decide!";
            write(characters[index]->sd, response.c_str(), response.length());
        }
    }

    bool haveVillagerWon()
    {
        return werewolfCount == 0;
    }

    bool haveWerewolfWon()
    {
        return werewolfCount >= villagerCount;
    }

    void performNight(int fromId, int toIndex) {
        int paramsCount = idToCharacter[fromId]->nightActionParametersCount;
        if (paramsCount == 0) idToCharacter[fromId]->nightAction();
        else if (paramsCount == 1) {
            idToCharacter[fromId]->nightAction(characters[toIndex]);
        }
    }

    void performDay(int fromId, int toIndex) {
        int paramsCount = idToCharacter[fromId]->dayActionParametersCount;
        if (paramsCount == 0) idToCharacter[fromId]->dayAction();
        else if (paramsCount == 1) {
            idToCharacter[fromId]->dayAction(characters[toIndex]);
        }
    }

    void performVote(int fromId, int toIndex) {
        idToCharacter[fromId]->vote(characters[toIndex]);
    }

    void updateCharactersCount() {
        int newWerewolfCount = 0, newVillagerCount = 0;
        for (auto& character : characters) {
            if (character->isAlive) {
                if (character->getName() == "Werewolf") newWerewolfCount++;
                else newVillagerCount++;
            }
        }
        werewolfCount = newWerewolfCount;
        villagerCount = newVillagerCount;
    }
};
