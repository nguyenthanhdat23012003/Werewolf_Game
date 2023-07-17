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
    Game* game;  // Private property to store the Game object

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

        // Create a Game instance
        game = new Game(playersId);

        // Send game start message to all players
        std::string startMessage = "The game has started!";
        for (const auto& player : players) {
            int playerId = player.first;
            write(playerId, startMessage.c_str(), startMessage.length());
        }

        do {
            sendGameStatus();
            promptNight();
            this_thread::sleep_for(chrono::seconds(30)); // Delay for 30 seconds
//            game->status();
//            game->promptDay();
//            this_thread::sleep_for(chrono::seconds(30));
            sendGameStatus();
            promptVote();
            this_thread::sleep_for(chrono::seconds(30));
            sendGameStatus();
        } while (!game->haveVillagerWon() && !game->haveWerewolfWon());

        string endMessage = "End game";
        for (const auto& player : players) {
            int playerId = player.first;
            write(playerId, endMessage.c_str(), endMessage.length());
        }
    }

    void sendGameStatus() {
        if (isGameStarted) {
            game->status();
            return;
        }
    }

    void promptNight() {
        if (isGameStarted) {
            game->promptNight();
            return;
        }
    }

    void performNight(int fromId, int toIndex) {
        if (isGameStarted) {
            game->performNight(fromId, toIndex);
            return;
        }
    }

    void promptDay() {
        if (isGameStarted) {
            game->promptDay();
            return;
        }
    }

    void performDay(int fromId, int toIndex) {
        if (isGameStarted) {
            game->performDay(fromId, toIndex);
            return;
        }
    }

    void promptVote() {
        if (isGameStarted) {
            game->promptVote();
            return;
        }
    }

    void performVote(int fromId, int toIndex) {
        if (isGameStarted) {
            game->performVote(fromId, toIndex);
            return;
        }
    }
};
