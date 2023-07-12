#include <algorithm>
#include "Character.h"

class Game
{
private:
    int n_players;
    int werewolfCount;              // Number of werewolves
    int villagerCount;              // Number of villagers
    vector<pair<int, Character *>> characters; // Stores the characters of the players
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
        }

        for (int i = 0; i < numSeers; i++)
        {
            pair<int, Character *> temp;
            temp.second = new Seer();
            temp.first = playersId[playerIdIndex--];
            characters.push_back(temp);
        }

        for (int i = 0; i < numVillagers; i++)
        {
            pair<int, Character *> temp;
            temp.second = new Villager();
            temp.first = playersId[playerIdIndex--];
            characters.push_back(temp);
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
        std::string status = "GAME STATUS\n";
        status += "Werewolf " + std::to_string(werewolfCount) + " VS " + std::to_string(villagerCount) + " Villager\n";
        status += "Up-to-date list of characters:\n";
        for (auto& character : characters)
        {
            status += "\t" + character.second->getName() + ((character.second->isAlive) ? " alive\n" : " dead\n");
        }
        status += "END STATUS\n";
        for (auto id : playersId) {
            write(id, status.c_str(), status.length());
        }
    }

    void night()
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
                cout << "You are " << character.second->getName() << endl;
                cout << "Enter the target's index for night action: ";
                int targetIndex;
                cin >> targetIndex;
                character.second->nightAction(characters[targetIndex].second);
            }
            else
            {
                cout << "Something wrong!\n";
            }
        }
    }

    void day()
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
                cout << "You are " << character.second->getName() << endl;
                cout << "Enter the target's index for day action: ";
                int targetIndex;
                cin >> targetIndex;
                character.second->dayAction(characters[targetIndex].second);
            }
            else
            {
                cout << "Something wrong!\n";
            }
        }
    }

    void vote()
    {
        for (auto& character : characters)
        {
            if (!character.second->isAlive)
                continue;

            cout << "Enter the target's index for voting: ";
            int targetIndex;
            cin >> targetIndex;
            character.second->vote(characters[targetIndex].second);
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
        cout << (maxVoteUnique == true) << endl;
        if (maxVoteUnique) {characters[index].second->isAlive = false; cout << index << " " << characters[index].second->getName() << endl;}
    }

    bool haveVillagerWon()
    {
        return werewolfCount == 0;
    }

    bool haveWerewolfWon()
    {
        return werewolfCount >= villagerCount;
    }
};
