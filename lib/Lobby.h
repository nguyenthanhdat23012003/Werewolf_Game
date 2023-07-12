#include <vector>
#include <unordered_map>
#include "Game.h"

enum class PlayerStatus {
    NotReady,
    Ready
};

class Lobby {
private:
    int id;
    std::unordered_map<int, PlayerStatus> players;  // Map to store player IDs and their status
    bool isGameStarted;

public:
    Lobby() {
        static int nextId = 10;
        id = nextId++;
        isGameStarted = false;
    }

    int getId() const {
        return id;
    }

    void addPlayer(int playerId) {
        players[playerId] = PlayerStatus::NotReady;  // New player is initially not ready
    }

    void removePlayer(int playerId) {
        players.erase(playerId);
    }

    bool hasPlayer(int playerId) const {
        return players.count(playerId) > 0;
    }

    PlayerStatus getPlayerStatus(int playerId) const {
        return players.at(playerId);
    }

    void setPlayerStatus(int playerId, PlayerStatus status) {
        players[playerId] = status;
    }

    bool isReadyToStartGame() const {
//        if (players.size() < 5) {
//            cout << "Lobby should have at least 5 players\n";
//            return false;  // Lobby should have at least 5 players
//        }

        for (const auto& player : players) {
            if (player.second != PlayerStatus::Ready) {
                cout << "Not all players are ready\n";
                return false;  // If any player is not ready, game cannot start
            }
        }

        return true;
    }

    void startGame() {
        if (isGameStarted) {
            // Game is already started
            return;
        }

        if (!isReadyToStartGame()) {
            // Not enough players or players not ready to start the game
            return;
        }

        isGameStarted = true;

        // create a vector of players' id so that Game.h can send and message to them
        std::vector<int> playersId;
        for(auto kv : players) {
            playersId.push_back(kv.first);
        }

        // Create a Game instance and start the game
        Game game(playersId);  // Assuming `Game` is defined and the constructor takes the number of players as an argument
        // Perform any necessary game initialization here

        // Start the game loop or call the appropriate game methods

        // Example:
        game.status();  // Print the initial status

        // Send game start message to all players
        std::string startMessage = "The game has started!";
        for (const auto& player : players) {
            int playerId = player.first;
            write(playerId, startMessage.c_str(), startMessage.length());
        }

        while (!game.haveWerewolfWon() && !game.haveVillagerWon()) {
            game.night();  // Perform night actions
            game.day();    // Perform day actions
            game.vote();   // Perform voting
            game.status(); // Print the updated status
        }

        // Game has ended, perform any necessary cleanup or post-game actions
    }
};
