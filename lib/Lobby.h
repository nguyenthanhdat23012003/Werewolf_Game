#include <vector>
#include <unordered_map>
#include "Game.h"
#include "json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

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

    int getLobbySize() const {
        return players.size();
    }

    bool getIsGameStarted() const {
        return isGameStarted;
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

        // Cleanup the old game
        if (game) {
            delete game;
            game = nullptr;
        }

        // Create a Game instance
        game = new Game(playersId);

        // Send game start message to all players
//        std::string startMessage = "The game has started!";
//        for (const auto& player : players) {
//            int playerId = player.first;
//            write(playerId, startMessage.c_str(), startMessage.length());
//        }
        do {
            this_thread::sleep_for(chrono::seconds(10));
            if (game->getIsDay()) processVote();
            game->toggleIsDay();
        } while (!isGameEnded());
//        sendGameStatus();
        endGame();
    }

    void appendToInGameChatHistory(int from, string content) {
        game->appendToChatHistory(from, content);
    }

    void sendGameStatus() {
        if (isGameStarted) {
            game->status();
            return;
        }
    }

    void sendGameStatusTo(int sd) {
            game->statusTo(sd);
    }

    void performNight(int fromId, int toId) {
        if (isGameStarted) {
            game->performNight(fromId, toId);
            return;
        }
    }

    void performDay(int fromId, int toIndex) {
        if (isGameStarted) {
            game->performDay(fromId, toIndex);
            return;
        }
    }

    void performVote(int fromId, int toId) {
        if (isGameStarted) {
            game->performVote(fromId, toId);
            return;
        }
    }

    void processVote() {
       if (isGameStarted) {
            game->processVote();
            return;
        }
    }

    bool isGameEnded() {
        return game->haveVillagerWon() || game->haveWerewolfWon();
    }

    void endGame() {
        if (isGameStarted) {
            // Reset player statuses to NotReady
            for (auto& player : players) {
                player.second = PlayerStatus::NotReady;
            }

            // Reset the game started flag
            isGameStarted = false;
        }
    }

    // Method to convert the Lobby to a JSON object
    json toJson() const {
        json lobbyJson;
        lobbyJson["isGameStarted"] = isGameStarted;

        // Create a JSON array for players
        json playersJson = json::array();
        for (const auto& player : players) {
            json playerJson;
            playerJson["id"] = player.first;
            playerJson["isReady"] = (player.second == PlayerStatus::Ready);
            playersJson.push_back(playerJson);
        }
        lobbyJson["players"] = playersJson;

        return lobbyJson;
    }
};
