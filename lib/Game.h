#include "Character.h"

class Game
{
private:
    int n_players;
    int werewolfCount;              // Number of werewolves
    int villagerCount;              // Number of villagers
    vector<Character *> characters; // Stores the characters of the players

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
            characters.push_back(new Werewolf());
        }

        for (int i = 0; i < numSeers; i++)
        {
            characters.push_back(new Seer());
        }

        for (int i = 0; i < numVillagers; i++)
        {
            characters.push_back(new Villager());
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
        for (Character *character : characters)
        {
            cout << "\t" << character->getName() << ((character->isAlive) ? " alive" : " dead") << endl;
        }
        cout << "END STATUS\n";
    }

    void night()
    {
        for (Character *character : characters)
        {
            if (!character->isAlive)
                continue;

            if (character->nightActionParametersCount == 0)
            {
                character->nightAction();
            }
            else if (character->nightActionParametersCount == 1)
            {
                cout << "You are " << character->getName() << endl;
                cout << "Enter the target's index for night action: ";
                int targetIndex;
                cin >> targetIndex;
                character->nightAction(characters[targetIndex]);
            }
            else
            {
                cout << "Something wrong!\n";
            }
        }
    }

    void day()
    {
        for (Character *character : characters)
        {
            if (!character->isAlive)
                continue;

            if (character->dayActionParametersCount == 0)
            {
                character->dayAction();
            }
            else if (character->dayActionParametersCount == 1)
            {
                cout << "You are " << character->getName() << endl;
                cout << "Enter the target's index for day action: ";
                int targetIndex;
                cin >> targetIndex;
                character->dayAction(characters[targetIndex]);
            }
            else
            {
                cout << "Something wrong!\n";
            }
        }
    }

    void vote()
    {
        for (Character *character : characters)
        {
            if (!character->isAlive)
                continue;

            cout << "Enter the target's index for voting: ";
            int targetIndex;
            cin >> targetIndex;
            character->vote(characters[targetIndex]);
        }

        int index = 0;
        int maxVote = characters[0]->voteCount;
        bool maxVoteUnique = true;
        for (int i = 1; i < n_players; i++)
        {
            if (characters[i]->voteCount > maxVote)
            {
                maxVote = characters[i]->voteCount;
                index = i;
                maxVoteUnique = true;
            }
            else if (characters[i]->voteCount == maxVote)
            {
                maxVoteUnique = false;
            }
        }
        cout << (maxVoteUnique == true) << endl;
        if (maxVoteUnique) {characters[index]->isAlive = false; cout << index << " " << characters[index]->getName() << endl;}
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
