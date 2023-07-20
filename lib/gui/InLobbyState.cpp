#include "InLobbyState.h"

InLobbyState::InLobbyState(SDL_Window* window, TTF_Font* font) : renderer(window, font) {
    inLobbyData = nullptr;
}

void InLobbyState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        // Check if the mouse click is within the "START" button
        const SDL_Rect startButtonRect = { 1000, 550, 100, 30 };
        if (renderer.isPointInRect(mouseX, mouseY, startButtonRect)) {
            printf("Clicked on START button!\n");
            GameState::setCurrentState(GameState::State::IN_GAME);
            // Perform actions when the "START" button is clicked
        }

        // Check if the mouse click is within the "READY" button
        const SDL_Rect readyButtonRect = { 1000, 600, 100, 30 };
        if (renderer.isPointInRect(mouseX, mouseY, readyButtonRect)) {
            printf("Clicked on READY button!\n");
            // Perform actions when the "READY" button is clicked
            requestHelper.sendRequest(to_string(CommandMessage::READY));
        }

        // Check if the mouse click is within the "LEAVE" button
        const SDL_Rect leaveButtonRect = { 1000, 650, 100, 30 };
        if (renderer.isPointInRect(mouseX, mouseY, leaveButtonRect)) {
            printf("Clicked on LEAVE button!\n");
            GameState::setCurrentState(GameState::State::LOBBY_LIST);
        }
    }
}

void InLobbyState::update() {
    string response = requestHelper.sendRequest(to_string(CommandMessage::IN_LOBBY));
    try {
        json jsonData = json::parse(response);
        inLobbyData = jsonData;
    } catch (const json::exception& e) {
        cerr << "Failed to parse JSON response: " << e.what() << endl;
    }
}

void InLobbyState::render() {
    // Draw the players table in lobby
    // Draw table header
    renderer.drawText("ID", 200, 50, 255, 255, 255);
    renderer.drawText("Status", 400, 50, 255, 255, 255);

    // Draw table data from the JSON array
    int i = 0;
    for (const auto& player : inLobbyData["players"]) {
        // Assuming the JSON object contains "id" and "isReady" fields
        int id = player["id"];
        bool isReady = player["isReady"];

        // Convert the bool to a string to display in the table
        std::string status = isReady ? "Ready" : "Not Ready";

        renderer.drawText(std::to_string(id), 200, 100 + i * 50, 255, 255, 255);
        renderer.drawText(status, 400, 100 + i * 50, 255, 255, 255);
        i++;
    }

    // Draw START, READY, and LEAVE buttons
    renderer.drawRect(1000, 550, 100, 30, 255, 255, 255);
    renderer.drawText("START", 1000 + 10, 550, 0, 0, 0);
    renderer.drawRect(1000, 600, 100, 30, 255, 255, 255);
    renderer.drawText("READY", 1000 + 10, 600, 0, 0, 0);
    renderer.drawRect(1000, 650, 100, 30, 255, 255, 255);
    renderer.drawText("LEAVE", 1000 + 10, 650, 0, 0, 0);

    SDL_UpdateWindowSurface(renderer.getWindow());
}
