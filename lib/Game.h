#include "Character.h"

class Game
{
private:
    int n_players;
    int werewolfCount;              // Number of werewolves
    int villagerCount;              // Number of villagers
    vector<pair<int, Character *>> characters; // Stores the characters of the players

    void initializeCharacters()
    {
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
            characters.push_back(temp);
        }

        for (int i = 0; i < numSeers; i++)
        {
            pair<int, Character *> temp;
            temp.second = new Seer();
            characters.push_back(temp);
        }

        for (int i = 0; i < numVillagers; i++)
        {
            pair<int, Character *> temp;
            temp.second = new Villager();
            characters.push_back(temp);
        }
    }

public:
    Game(int numPlayers) : n_players(numPlayers)
    {
        // Initialize the characters based on the number of players
        initializeCharacters();
    }

    void status()
    {
        cout << "\nSTATUS\n";
        cout << "Werewolf " << werewolfCount << " VS " << villagerCount << " Villager" << endl;
        cout << "Up-to-date list of characters:\n";
        for (auto& character : characters)
        {
            cout << "\t" << character.second->getName() << ((character.second->isAlive) ? " alive" : " dead") << endl;
        }
        cout << "END STATUS\n";
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
