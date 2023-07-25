#pragma GCC diagnostic ignored "-Wsign-compare"
#include "InGameState.h"

InGameState::InGameState(SDL_Window* window, TTF_Font* font, RequestHelper* helper) : renderer(window, font), requestHelper(helper) {
    // Initialize players data
    inGameData = nullptr;

    isDay = false; // Set initial time data to "day"
    firstRender = true;
    lastDayChangeTime = SDL_GetTicks();
}

void InGameState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        // Check if the mouse click is within the "VOTE" or "NIGHT ACTION" button
        for (int i = 0; i < inGameData["players"].size(); ++i) {
            if (isDay) {
                const SDL_Rect voteButtonRect = { 800, 100 + i * 50, 100, 30 };
                if (renderer.isPointInRect(mouseX, mouseY, voteButtonRect)) {
                    std::cout <<"Clicked on VOTE button for player " << inGameData["players"][i]["id"] << std::endl;
                    // Perform actions when the "VOTE" button is clicked
                    int targetId = inGameData["players"][i]["id"];
                    std::string voteMessage = std::to_string(GameMessage::VOTE) + " " + std::to_string(targetId);
                    std::string response = requestHelper->sendRequest(voteMessage);
                }
            } else {
                const SDL_Rect nightActionButtonRect = { 800, 100 + i * 50, 180, 30 };
                if (renderer.isPointInRect(mouseX, mouseY, nightActionButtonRect)) {
                    std::cout <<"Clicked on NIGHT ACTION button for player " << inGameData["players"][i]["id"] << std::endl;
                    // Perform actions when the "NIGHT ACTION" button is clicked
                    int targetId = inGameData["players"][i]["id"];
                    std::string nightActionMessage = std::to_string(GameMessage::NIGHT_ACTION) + " " + std::to_string(targetId);
                    std::string response = requestHelper->sendRequest(nightActionMessage);
                }
            }
        }
    }
}


void InGameState::update() {
    if (firstRender || inGameData["isGameEnded"] == true) return;

    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - lastDayChangeTime >= DAY_CHANGE_INTERVAL) {
        std::string response = requestHelper->sendRequest(std::to_string(CommandMessage::IN_GAME));
        std::cout << response << std::endl;
        try {
            json jsonData = json::parse(response);
            inGameData = jsonData;
            if (inGameData["isDay"] != isDay) {
                std::cout << "Toggle\n";
                isDay = !isDay; // Toggle the day state
                lastDayChangeTime = currentTime; // Update the last day change time
            }
        } catch (const json::exception& e) {
            std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
        }
    }
}

void InGameState::render() {
    if (firstRender) {
        std::string response = requestHelper->sendRequest(std::to_string(CommandMessage::IN_GAME));
        std::cout << response << std::endl;
        try {
            json jsonData = json::parse(response);
            inGameData = jsonData;
        } catch (const json::exception& e) {
            std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
        }
        firstRender = false;
    }
    // Draw the players table in the game
    // Draw table header
    renderer.drawText("Index", 200, 50, 255, 255, 255);
    renderer.drawText("Character", 400, 50, 255, 255, 255);
    renderer.drawText("Status", 600, 50, 255, 255, 255);

    // Draw table data
    int index = 1; // Start the index from 1
    for (const auto& player : inGameData["players"]) {
        std::string character = player["character"];
        bool isAlive = player["isAlive"];

        std::string status = isAlive ? "Alive" : "Dead";

        renderer.drawText(std::to_string(index), 200, 100 + (index - 1) * 50, 255, 255, 255);
        if (player["id"] == inGameData["from"]) {
            renderer.drawRect(100, 100 + (index - 1) * 50, 20, 20, 0, 255, 0); // Green square to indicate the player's character
            renderer.drawText(character, 400, 100 + (index - 1) * 50, 255, 255, 255);
        } else {
            if (inGameData["yourCharacter"] == "Seer" && (std::find(inGameData["seenList"].begin(), inGameData["seenList"].end(), player["id"]) != inGameData["seenList"].end())) {
                std::string character = player["character"];
                renderer.drawText(character, 400, 100 + (index - 1) * 50, 255, 255, 255);
            } else {
                renderer.drawText("Hidden", 400, 100 + (index - 1) * 50, 255, 255, 255);
            }
            // Conditionally render "VOTE" or "NIGHT ACTION" button
            if (isDay) {
                renderer.drawRect(800, 100 + (index - 1) * 50, 100, 30, 255, 255, 255);
                renderer.drawText("VOTE", 800 + 20, 100 + (index - 1) * 50, 0, 0, 0);
            } else {
                renderer.drawRect(800, 100 + (index - 1) * 50, 180, 30, 255, 255, 255);
                renderer.drawText("NIGHT ACTION", 800 + 10, 100 + (index - 1) * 50, 0, 0, 0);
            }
        }
        renderer.drawText(status, 600, 100 + (index - 1) * 50, 255, 255, 255);
        index++;
    }

    if (inGameData["isGameEnded"]) {
        renderer.drawRect(1280/2-100, 720/2-10, 200, 30, 255, 255, 255);
        renderer.drawText("GAME END!", 1280/2-80, 720/2-10, 0, 0, 0);
    }

    // Update the window surface
    SDL_UpdateWindowSurface(renderer.getWindow());
}

