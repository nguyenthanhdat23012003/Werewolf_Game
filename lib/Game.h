#include <algorithm>
#include "Character.h"

class Game
{
private:
    int n_players;
    int werewolfCount;              // Number of werewolves
    int villagerCount;              // Number of villagers
    vector<pair<int, Character *>> characters; // Stores the characters of the players
    unordered_map<int, Character*> idToCharacter;
    vector<int> playersId;

    void initializeCharacters()
    {
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
        for (int i = 0; i < numWerewolves; i++)
        {
            pair<int, Character *> temp;
            temp.second = new Werewolf();
            temp.first = playersId[playerIdIndex--];
            characters.push_back(temp);
            idToCharacter[temp.first] = temp.second; // Insert into the unordered_map
        }

        for (int i = 0; i < numSeers; i++)
        {
            pair<int, Character *> temp;
            temp.second = new Seer();
            temp.first = playersId[playerIdIndex--];
            characters.push_back(temp);
            idToCharacter[temp.first] = temp.second; // Insert into the unordered_map
        }

        for (int i = 0; i < numVillagers; i++)
        {
            pair<int, Character *> temp;
            temp.second = new Villager();
            temp.first = playersId[playerIdIndex--];
            characters.push_back(temp);
            idToCharacter[temp.first] = temp.second; // Insert into the unordered_map
        }
    }

public:
    Game(vector<int> playersId) : n_players(playersId.size()), playersId(playersId)
    {
        // Initialize the characters based on the number of players
        initializeCharacters();
    }

    void status()
    {
        string status = "\n------------------------------\n";
        status += "Index\tCharacter\tStatus\n";
        int count = 0;
        for (auto& character : characters)
        {
            string characterStatus = (character.second->isAlive) ? "Alive" : "Dead";
            string row = to_string(count++) + "\t" + character.second->getName() + "\t" + characterStatus + "\n";
            status += row;
        }
        status += "------------------------------\n";
        for (auto id : playersId) {
            write(id, status.c_str(), status.length());
        }
    }

    void promptNight()
    {
        for (auto& character : characters)
        {
            if (!character.second->isAlive)
                continue;

            if (character.second->nightActionParametersCount == 0)
            {
                character.second->nightAction();
            }
            else if (character.second->nightActionParametersCount == 1)
            {
                string response = "You are " + character.second->getName() + "\nEnter the target\'s index for night action: ";
                write(character.first, response.c_str(), response.length());
//                cout << "You are " << character.second->getName() << endl;
//                cout << "Enter the target's index for night action: ";
//                int targetIndex;
//                cin >> targetIndex;
//                character.second->nightAction(characters[targetIndex].second);
            }
            else
            {
                string response = "Something wrong!\n";
                write(character.first, response.c_str(), response.length());
            }
        }
    }

    void promptDay()
    {
        for (auto& character : characters)
        {
            if (!character.second->isAlive)
                continue;

            if (character.second->dayActionParametersCount == 0)
            {
                character.second->dayAction();
            }
            else if (character.second->dayActionParametersCount == 1)
            {
                string response = "You are " + character.second->getName() + "\nEnter the target\'s index for day action: ";
                write(character.first, response.c_str(), response.length());
//                cout << "You are " << character.second->getName() << endl;
//                cout << "Enter the target's index for day action: ";
//                int targetIndex;
//                cin >> targetIndex;
//                character.second->dayAction(characters[targetIndex].second);
            }
            else
            {
                string response = "Something wrong!\n";
                write(character.first, response.c_str(), response.length());
            }
        }
    }

    void promptVote()
    {
        for (auto& character : characters)
        {
            if (!character.second->isAlive)
                continue;

            string response=  "Enter the target's index for voting: ";
            write(character.first, response.c_str(), response.length());
//            int targetIndex;
//            cin >> targetIndex;
//            character.second->vote(characters[targetIndex].second);
        }

        int index = 0;
        int maxVote = characters[0].second->voteCount;
        bool maxVoteUnique = true;
        for (int i = 1; i < n_players; i++)
        {
            if (characters[i].second->voteCount > maxVote)
            {
                maxVote = characters[i].second->voteCount;
                index = i;
                maxVoteUnique = true;
            }
            else if (characters[i].second->voteCount == maxVote)
            {
                maxVoteUnique = false;
            }
        }
        if (maxVoteUnique) {
            characters[index].second->isAlive = false;
            string response = "Player index " + to_string(index) + " is hung by villagers!";
            write(characters[index].first, response.c_str(), response.length());
        }
        else {
            string response = "Villagers couldn't decide!";
            write(characters[index].first, response.c_str(), response.length());
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
            idToCharacter[fromId]->nightAction(characters[toIndex].second);
        }
    }

    void performDay(int fromId, int toIndex) {
        int paramsCount = idToCharacter[fromId]->dayActionParametersCount;
        if (paramsCount == 0) idToCharacter[fromId]->dayAction();
        else if (paramsCount == 1) {
            idToCharacter[fromId]->dayAction(characters[toIndex].second);
        }
    }

    void performVote(int fromId, int toIndex) {
        idToCharacter[fromId]->vote(characters[toIndex].second);
    }
};
