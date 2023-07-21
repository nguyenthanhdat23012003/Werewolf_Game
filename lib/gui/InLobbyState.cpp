#include "InLobbyState.h"

InLobbyState::InLobbyState(SDL_Window* window, TTF_Font* font, RequestHelper* helper) : renderer(window, font), requestHelper(helper) {
    // Initialize players data
    inLobbyData = nullptr;
    firstRender = true;
}

void InLobbyState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;
        // Check if the mouse click is within the "START" button
        const SDL_Rect startButtonRect = { 1000, 550, 100, 30 };
        if (renderer.isPointInRect(mouseX, mouseY, startButtonRect)) {
            printf("Clicked on START button!\n");
            std::string response = requestHelper->sendRequest(std::to_string(CommandMessage::START));
            std::cout << response << std::endl;
            json jsonData = json::parse(response);
            if (jsonData["message"] == "success")
                GameState::setCurrentState(GameState::State::IN_GAME);
            // Perform actions when the "START" button is clicked
        }

        // Check if the mouse click is within the "READY" button
        const SDL_Rect readyButtonRect = { 1000, 600, 100, 30 };
        if (renderer.isPointInRect(mouseX, mouseY, readyButtonRect)) {
            printf("Clicked on READY button!\n");
            // Perform actions when the "READY" button is clicked
            requestHelper->sendRequest(std::to_string(CommandMessage::READY));
            firstRender = true;
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
    // Update the state if needed
}

void InLobbyState::render() {
    if (firstRender) {
        std::string response = requestHelper->sendRequest(std::to_string(CommandMessage::IN_LOBBY));
        std::cout << response << std::endl;
        try {
            json jsonData = json::parse(response);
            inLobbyData = jsonData;
        } catch (const json::exception& e) {
            std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
        }

        firstRender = false;
    }

    // Draw the players table in lobby
    // Draw table header
    renderer.drawText("ID", 200, 50, 255, 255, 255);
    renderer.drawText("Status", 400, 50, 255, 255, 255);

    // Draw table data
    for (int i = 0; i < inLobbyData["players"].size(); ++i) {
        // Access the "id" and "isReady" fields from the JSON object
        int id = inLobbyData["players"][i]["id"];
        bool isReady = inLobbyData["players"][i]["isReady"];

        // Convert the bool to a string to display in the table
        std::string status = isReady ? "Ready" : "Not Ready";

        renderer.drawText(std::to_string(id), 200, 100 + i * 50, 255, 255, 255);
        renderer.drawText(status, 400, 100 + i * 50, 255, 255, 255);
    }

    // Draw LEAVE and READY buttons
    renderer.drawRect(1000, 550, 100, 30, 255, 255, 255);
    renderer.drawText("START", 1000 + 10, 550, 0, 0, 0);
    renderer.drawRect(1000, 600, 100, 30, 255, 255, 255);
    renderer.drawText("READY", 1000 + 10, 600, 0, 0, 0);
    renderer.drawRect(1000, 650, 100, 30, 255, 255, 255);
    renderer.drawText("LEAVE", 1000 + 10, 650, 0, 0, 0);

    SDL_UpdateWindowSurface(renderer.getWindow());
}
