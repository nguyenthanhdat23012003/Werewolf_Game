#include "InGameState.h"

InGameState::InGameState(SDL_Window* window, TTF_Font* font) : renderer(window, font) {
    // Initialize players data
    playersData = {
            { "1", "Werewolf", "Alive" },
            { "2", "Villager", "Dead" },
            // Add more players data as needed
    };

    currentTimeData = "day"; // Set initial time data to "day"
}

void InGameState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        // Check if the mouse click is within the "VOTE" or "NIGHT ACTION" button
        for (int i = 0; i < playersData.size(); ++i) {
            if (currentTimeData == "day") {
                const SDL_Rect voteButtonRect = { 800, 100 + i * 50, 100, 30 };
                if (renderer.isPointInRect(mouseX, mouseY, voteButtonRect)) {
                    printf("Clicked on VOTE button for player %s!\n", playersData[i][1].c_str());
                    // Perform actions when the "VOTE" button is clicked
                }
            } else if (currentTimeData == "night") {
                const SDL_Rect nightActionButtonRect = { 800, 100 + i * 50, 180, 30 };
                if (renderer.isPointInRect(mouseX, mouseY, nightActionButtonRect)) {
                    printf("Clicked on NIGHT ACTION button for player %s!\n", playersData[i][1].c_str());
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
    // Draw the players table in the game
    // Draw table header
    renderer.drawText("Index", 200, 50, 255, 255, 255);
    renderer.drawText("Character", 400, 50, 255, 255, 255);
    renderer.drawText("Status", 600, 50, 255, 255, 255);

    // Draw table data
    for (int i = 0; i < playersData.size(); ++i) {
        renderer.drawText(playersData[i][0], 200, 100 + i * 50, 255, 255, 255);
        renderer.drawText(playersData[i][1], 400, 100 + i * 50, 255, 255, 255);
        renderer.drawText(playersData[i][2], 600, 100 + i * 50, 255, 255, 255);

        // Conditionally render "VOTE" button if currentTimeData is "day"
        if (currentTimeData == "day") {
            renderer.drawRect(800, 100 + i * 50, 100, 30, 255, 255, 255);
            renderer.drawText("VOTE", 800 + 20, 100 + i * 50, 0, 0, 0);
        } else if (currentTimeData == "night") {
            renderer.drawRect(800, 100 + i * 50, 180, 30, 255, 255, 255);
            renderer.drawText("NIGHT ACTION", 800 + 10, 100 + i * 50, 0, 0, 0);
        }
    }

    // Update the window surface
    SDL_UpdateWindowSurface(renderer.getWindow());
}
