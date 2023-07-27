#pragma GCC diagnostic ignored "-Wsign-compare"
#include "InGameState.h"
#include <algorithm>
#include <random>

InGameState::InGameState(SDL_Window* window, TTF_Font* font, RequestHelper* helper) : renderer(window, font), requestHelper(helper) {
    // Initialize players data
    inGameData = nullptr;

    isDay = false; // Set initial time data to "day"
    firstRender = true;
    lastDayChangeTime = SDL_GetTicks();
    isChatInputFocused = false;
    chatInputText = "";
    hasActed = false;
}

void InGameState::handleTextInputEvent(SDL_Event& e) {
    if (isChatInputFocused && e.type == SDL_TEXTINPUT) {
        // Append the received text to the chat input text
        chatInputText += e.text.text;
    } else if (isChatInputFocused && e.type == SDL_KEYDOWN) {
        // Handle special keys like backspace
        if (e.key.keysym.sym == SDLK_BACKSPACE && !chatInputText.empty()) {
            // Erase the last character from the chat input text
            chatInputText.pop_back();
        }
    }
}

void InGameState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        // Check if the mouse click is within the "VOTE" or "NIGHT ACTION" button
        for (int i = 0; i < inGameData["players"].size(); ++i) {
            if (isDay) {
                const SDL_Rect voteButtonRect = { 700, 100 + i * 50, 100, 30 };
                if (renderer.isPointInRect(mouseX, mouseY, voteButtonRect)) {
                    std::cout <<"Clicked on VOTE button for player " << inGameData["players"][i]["id"] << std::endl;
                    // Perform actions when the "VOTE" button is clicked
                    int targetId = inGameData["players"][i]["id"];
                    std::string voteMessage = std::to_string(GameMessage::VOTE) + " " + std::to_string(targetId);
                    std::string response = requestHelper->sendRequest(voteMessage);
                    hasActed = true;
                }
            } else {
                const SDL_Rect nightActionButtonRect = { 700, 100 + i * 50, 100, 30 };
                if (renderer.isPointInRect(mouseX, mouseY, nightActionButtonRect)) {
                    std::cout <<"Clicked on NIGHT ACTION button for player " << inGameData["players"][i]["id"] << std::endl;
                    // Perform actions when the "NIGHT ACTION" button is clicked
                    int targetId = inGameData["players"][i]["id"];
                    std::string nightActionMessage = std::to_string(GameMessage::NIGHT_ACTION) + " " + std::to_string(targetId);
                    std::string response = requestHelper->sendRequest(nightActionMessage);
                    hasActed = true;
                }
            }
        }
    }
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        const SDL_Rect chatInputBoxRect = { 950, 460, 250, 30 };
        if (renderer.isPointInRect(mouseX, mouseY, chatInputBoxRect)) {
            isChatInputFocused = true;
        } else {
            isChatInputFocused = false;
        }
    }else if (e.type == SDL_TEXTINPUT) {
        // Handle text input events when chat input is focused
        handleTextInputEvent(e);
    } else if (e.type == SDL_KEYDOWN) {
        // Handle key press events
        if (isChatInputFocused) {
            if (e.key.keysym.sym == SDLK_RETURN) {
                // Send chat message to the server
                if (!chatInputText.empty()) {
                    std::string chatMessage = std::to_string(GameMessage::CHAT) + " " + chatInputText;
                    std::string response = requestHelper->sendRequest(chatMessage);
                    // Optionally, handle the response from the server if needed
                    // ...
                    // Clear the chat input text after sending the message
                    chatInputText = "";
                }
            } else if (e.key.keysym.sym == SDLK_BACKSPACE && !chatInputText.empty()) {
                // Handle backspace key press when chat input is focused
                chatInputText.pop_back();
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
                hasActed = false;
            }
        } catch (const json::exception& e) {
            std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
        }
        lastDayChangeTime = currentTime; // Update the last day change time
    }

//    // Request GET_CHAT every 1 second
//    std::cout << "Flag\n";
//    if (currentTime - lastChatRequestTime >= CHAT_REQUEST_INTERVAL) {
//        std::string response = requestHelper->sendRequest(std::to_string(GameMessage::GET_CHAT));
//        std::cout << response << std::endl;
//        try {
//            json jsonData = json::parse(response);
//            chatMessages = jsonData; // Save the chat messages received from the server
//        } catch (const json::exception& e) {
//            std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
//        }
//        lastChatRequestTime = currentTime; // Update the last chat request time
//    }
}

void InGameState::render() {
    static std::vector<int> randomIndexOrder = {};
    if (firstRender) {
        std::cout << "First InGameState Render" << std::endl;
        std::string response = requestHelper->sendRequest(std::to_string(CommandMessage::IN_GAME));
        std::cout << response << std::endl;
        try {
            json jsonData = json::parse(response);
            inGameData = jsonData;
            for (int i = 0; i < inGameData["players"].size(); ++i) {
                randomIndexOrder.push_back(i);
            }
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(randomIndexOrder.begin(), randomIndexOrder.end(), g);
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
    for (int randomIndex : randomIndexOrder) {
        const auto& player = inGameData["players"][randomIndex];
        int playerId = player["id"]; // Get the Player ID from the data
        std::string character = player["character"];
        bool isAlive = player["isAlive"];

        std::string status = isAlive ? "Alive" : "Dead";

        renderer.drawText(std::to_string(playerId), 200, 100 + (index - 1) * 50, 255, 255, 255);
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
            if (!hasActed) {
                if (isDay) {
                    renderer.drawRect(700, 100 + (index - 1) * 50, 100, 30, 255, 255, 255);
                    renderer.drawText("VOTE", 700 + 20, 100 + (index - 1) * 50, 0, 0, 0);
                } else {
                    renderer.drawRect(700, 100 + (index - 1) * 50, 100, 30, 255, 255, 255);
                    renderer.drawText("ACTION", 700 + 10, 100 + (index - 1) * 50, 0, 0, 0);
                }
            }
        }
        renderer.drawText(status, 600, 100 + (index - 1) * 50, 255, 255, 255);
        index++;
    }

    //Draw the chat box and header
    renderer.drawRect(950, 50, 250, 400, 255, 255, 255);
    renderer.drawText("CHAT", 950 + 90, 50, 0, 0, 0);
    // Draw the chat input box
    if (isChatInputFocused) {
        renderer.drawRect(950, 460, 250, 30, 150, 0, 0);
    } else {
        renderer.drawRect(950, 460, 250, 30, 50, 0, 0);
    }
    renderer.drawText(chatInputText, 950 + 10, 460, 0, 0, 0);
    // Draw the chat messages
    int chatIndex = 0;
    for (const auto& chat : inGameData["chatHistory"]) {
        std::string message = chat["message"];
        int playerId = chat["from"];
        renderer.drawText(std::to_string(playerId) + ": " + message, 950 + 10, 90 + chatIndex * 30, 0, 0, 0);
        chatIndex++;
    }

    if (inGameData["isGameEnded"]) {
        renderer.drawRect(1280/2-75, 720/2-10, 150, 30, 255, 255, 255);
        renderer.drawText("GAME END!", 1280/2-60, 720/2-10, 0, 0, 0);
    }

    // Update the window surface
    SDL_UpdateWindowSurface(renderer.getWindow());
}

