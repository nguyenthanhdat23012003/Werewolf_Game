#include "LobbyListState.h"

LobbyListState::LobbyListState(SDL_Window* window, TTF_Font* font) : renderer(window, font) {
    // Initialize lobby data
    lobbyData = {
            { "1", "Waiting" },
            { "2", "In Game" },
            // Add more lobby data as needed
    };
}

void LobbyListState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        // Check for join button clicks in the lobby list
        for (int i = 0; i < lobbyData.size(); ++i) {
            const SDL_Rect joinButtonRect = { 600, 100 + i * 50, 100, 30 };
            if (renderer.isPointInRect(mouseX, mouseY, joinButtonRect)) {
                printf("Clicked on JOIN button!\n");
                GameState::setCurrentState(GameState::State::IN_LOBBY);
                break; // No need to check further, as we found the clicked join button
            }
        }
    }
}

void LobbyListState::update() {
    // Update the state if needed
}

void LobbyListState::render() {
    // Draw the lobby list table
    // Draw table header
    renderer.drawText("ID", 200, 50, 255, 255, 255);
    renderer.drawText("Status", 400, 50, 255, 255, 255);

    // Draw table data
    for (int i = 0; i < lobbyData.size(); ++i) {
        renderer.drawText(lobbyData[i][0], 200, 100 + i * 50, 255, 255, 255);
        renderer.drawText(lobbyData[i][1], 400, 100 + i * 50, 255, 255, 255);
        renderer.drawRect(600, 100 + i * 50, 100, 30, 255, 255, 255);
        renderer.drawText("JOIN", 600 + 20, 100 + i * 50, 0, 0, 0);
    }

    SDL_UpdateWindowSurface(renderer.getWindow());
}
