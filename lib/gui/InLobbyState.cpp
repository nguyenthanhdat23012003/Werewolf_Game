#include "InLobbyState.h"

InLobbyState::InLobbyState(SDL_Window* window, TTF_Font* font) : renderer(window, font) {
    // Initialize players data
    playersData = {
            { "1", "Ready" },
            { "2", "Not Ready" },
            // Add more players data as needed
    };
}

void InLobbyState::handleEvents(SDL_Event& e) {
    // Handle events specific to the InLobbyState
}

void InLobbyState::update() {
    // Update the state if needed
}

void InLobbyState::render() {
    // Draw the players table in the lobby
    // ... (similar to your drawTable function)

    SDL_UpdateWindowSurface(renderer.getWindow());
}
