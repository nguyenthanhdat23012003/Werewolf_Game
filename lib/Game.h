#include <algorithm>
#include "Character.h"
#include "json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

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

        // Create a JSON object for the game status
        json gameStatus;
        gameStatus["from"] = 5; // Replace 5 with the correct "from" player ID
        gameStatus["n_players"] = n_players;
        gameStatus["isGameEnded"] = false;
        gameStatus["message"] = "success";

        // Create a JSON array for players
        json playersJson = json::array();
        for (auto& character : characters) {
            json playerJson;
            playerJson["id"] = character->sd;
            playerJson["character"] = character->getName();
            playerJson["isAlive"] = character->isAlive;
            playerJson["voteCount"] = character->voteCount;
            playersJson.push_back(playerJson);
        }
        gameStatus["players"] = playersJson;

        // Convert the JSON object to a string
        std::string jsonStr = gameStatus.dump();

        // Send the JSON string to all players
        for (auto& character : characters) {
            if (character->isAlive) {
                write(character->sd, jsonStr.c_str(), jsonStr.length());
            }
        }
    }

    void promptNight() {
//        for (auto &character: characters) {
//            if (!character->isAlive)
//                continue;
//
//            if (character->nightActionParametersCount == 0) {
//                character->nightAction();
//            } else if (character->nightActionParametersCount == 1) {
//                string response =
//                        "You are " + character->getName() + "\nEnter the target\'s index for night action: ";
//                write(character->sd, response.c_str(), response.length());
////                cout << "You are " << character.second->getName() << endl;
////                cout << "Enter the target's index for night action: ";
////                int targetIndex;
////                cin >> targetIndex;
////                character.second->nightAction(characters[targetIndex].second);
//            } else {
//                string response = "Something wrong!\n";
//                write(character->sd, response.c_str(), response.length());
//            }
//        }
    }

    void promptDay() {
//        for (auto &character: characters) {
//            if (!character->isAlive)
//                continue;
//
//            if (character->dayActionParametersCount == 0) {
//                character->dayAction();
//            } else if (character->dayActionParametersCount == 1) {
//                string response =
//                        "You are " + character->getName() + "\nEnter the target\'s index for day action: ";
//                write(character->sd, response.c_str(), response.length());
////                cout << "You are " << character.second->getName() << endl;
////                cout << "Enter the target's index for day action: ";
////                int targetIndex;
////                cin >> targetIndex;
////                character.second->dayAction(characters[targetIndex].second);
//            } else {
//                string response = "Something wrong!\n";
//                write(character->sd, response.c_str(), response.length());
//            }
//        }
    }

    void promptVote() {
//        for (auto &character: characters) {
//            if (!character->isAlive)
//                continue;
//
//            string response = "Enter the target's index for voting: ";
//            write(character->sd, response.c_str(), response.length());
////            int targetIndex;
////            cin >> targetIndex;
////            character.second->vote(characters[targetIndex].second);
//        }
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
//            string response = "Player index " + to_string(index) + " is hung by villagers!";
//            write(characters[index]->sd, response.c_str(), response.length());
        } else {
//            string response = "Villagers couldn't decide!";
//            write(characters[index]->sd, response.c_str(), response.length());
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
