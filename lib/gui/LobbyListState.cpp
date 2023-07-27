#pragma GCC diagnostic ignored "-Wsign-compare"
#include "LobbyListState.h"

LobbyListState::LobbyListState(SDL_Window* window, TTF_Font* font, RequestHelper* helper) : renderer(window, font), requestHelper(helper) {
    string response = requestHelper->sendRequest(to_string(CommandMessage::NEW));
    cout << response << endl;
    // Parse the JSON response
    try {
        json jsonData = json::parse(response);
        lobbyData = jsonData;
    } catch (const json::exception& e) {
        cerr << "Failed to parse JSON response: " << e.what() << endl;
    }
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
                
                // Get the status of the room
                bool isGameStarted = lobbyData[i]["isGameStarted"];
                
                if (!isGameStarted) {
                    // If the room status is "Waiting," send the JOIN request to the server
                    requestHelper->sendRequest(to_string(CommandMessage::JOIN) + " " + to_string(lobbyData[i]["id"]));
                    GameState::setCurrentState(GameState::State::IN_LOBBY);
                } else {
                    // If the room status is "In Game," display a message that joining is not possible
                    printf("Joining this room is not possible because it's already in a game.\n");
                    // If the room status is "In Game," display a message that joining is not possible
                    const SDL_MessageBoxButtonData buttons[] = {
                        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" }
                    };

                    const SDL_MessageBoxColorScheme colorScheme = {
                        { /* .colors (.r, .g, .b) */
                            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                            { 255, 0, 0 },
                            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                            { 0, 255, 0 },
                            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                            { 255, 255, 0 },
                            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                            { 0, 0, 255 },
                            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                            { 255, 0, 255 }
                        }
                    };

                    const SDL_MessageBoxData messageboxdata = {
                        SDL_MESSAGEBOX_ERROR, /* .flags */
                        NULL, /* .window */
                        "Error", /* .title */
                        "Joining this room is not possible because it's already in a game.", /* .message */
                        SDL_arraysize(buttons), /* .numbuttons */
                        buttons, /* .buttons */
                        &colorScheme /* .colorScheme */
                    };

                    int buttonid;
                    SDL_ShowMessageBox(&messageboxdata, &buttonid);
                }

                break; // No need to check further, as we found the clicked join button
            }
        }

        // Check for click on the "REFRESH" button
        const SDL_Rect refreshButtonRect = { 1000, 600, 120, 30 };
        if (renderer.isPointInRect(mouseX, mouseY, refreshButtonRect)) {
            printf("Clicked on REFRESH button!\n");
            string response = requestHelper->sendRequest(to_string(CommandMessage::NEW));
            cout << response << endl;
            try {
                json jsonData = json::parse(response);
                lobbyData = jsonData;
            } catch (const json::exception& e) {
                cerr << "Failed to parse JSON response: " << e.what() << endl;
            }
        }

        // Check for click on the "CREATE ROOM" button
        const SDL_Rect createButtonRect = { 1000, 560, 120, 30 };
        if (renderer.isPointInRect(mouseX, mouseY, createButtonRect)) {
            printf("Clicked on CREATE ROOM button!\n");
            requestHelper->sendRequest(to_string(CommandMessage::CREATE));
            GameState::setCurrentState(GameState::State::IN_LOBBY);
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

    // Draw table data from the JSON array
    int i = 0;
    for (const auto& item : lobbyData) {
        // Assuming the JSON object contains "id" and "isGameStarted" fields
        int id = item["id"];
        bool isGameStarted = item["isGameStarted"];

        // Convert the bool to a string to display in the table
        std::string status = isGameStarted ? "In Game" : "Waiting";

        renderer.drawText(std::to_string(id), 200, 100 + i * 50, 255, 255, 255);
        renderer.drawText(status, 400, 100 + i * 50, 255, 255, 255);
        renderer.drawRect(600, 100 + i * 50, 100, 30, 255, 255, 255);
        renderer.drawText("JOIN", 600 + 20, 100 + i * 50, 0, 0, 0);
        i++;
    }

    renderer.drawRect(1000, 600, 120, 30, 255, 255, 255);
    renderer.drawText("REFRESH", 1000 + 10, 600, 0, 0, 0);
    renderer.drawRect(1000, 560, 120, 30, 255, 255, 255);
    renderer.drawText("CREATE", 1000 + 15, 560, 0, 0, 0);

    SDL_UpdateWindowSurface(renderer.getWindow());
}
