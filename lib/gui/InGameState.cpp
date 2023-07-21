#include "InGameState.h"

InGameState::InGameState(SDL_Window* window, TTF_Font* font, RequestHelper* helper) : renderer(window, font), requestHelper(helper) {
    // Initialize players data
    inGameData = {
            { "1", "Werewolf", "Alive" },
            { "2", "Villager", "Dead" },
            // Add more players data as needed
    };

    isDay = false; // Set initial time data to "day"
    firstRender = true;
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
                }
            } else {
                const SDL_Rect nightActionButtonRect = { 800, 100 + i * 50, 180, 30 };
                if (renderer.isPointInRect(mouseX, mouseY, nightActionButtonRect)) {
                    std::cout <<"Clicked on NIGHT ACTION button for player " << inGameData["players"][i]["id"] << std::endl;
                    // Perform actions when the "NIGHT ACTION" button is clicked
                }
            }
        }
    }
}


void InGameState::update() {
    // Update the state if needed
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
        int id = player["id"];
        bool isAlive = player["isAlive"];

        std::string status = isAlive ? "Alive" : "Dead";

        renderer.drawText(std::to_string(index), 200, 100 + (index - 1) * 50, 255, 255, 255);
        renderer.drawText(character, 400, 100 + (index - 1) * 50, 255, 255, 255);
        renderer.drawText(status, 600, 100 + (index - 1) * 50, 255, 255, 255);

        // Conditionally render "VOTE" or "NIGHT ACTION" button
        if (isDay) {
            renderer.drawRect(800, 100 + (index - 1) * 50, 100, 30, 255, 255, 255);
            renderer.drawText("VOTE", 800 + 20, 100 + (index - 1) * 50, 0, 0, 0);
        } else {
            renderer.drawRect(800, 100 + (index - 1) * 50, 180, 30, 255, 255, 255);
            renderer.drawText("NIGHT ACTION", 800 + 10, 100 + (index - 1) * 50, 0, 0, 0);
        }

        index++;
    }

    // Update the window surface
    SDL_UpdateWindowSurface(renderer.getWindow());
}

