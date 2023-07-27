#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "SDLRenderer.h"
#include "../RequestHelper.h"
#include <vector>
#include <iostream>
#include <string>
#include "../json-develop/single_include/nlohmann/json.hpp"
#include "../Message.h"

using json = nlohmann::json;

class InGameState {
public:
    InGameState(SDL_Window* window, TTF_Font* font, RequestHelper* helper);

    void handleTextInputEvent(SDL_Event& e);
    void handleEvents(SDL_Event& e);
    void update();
    void render();

private:
    SDLRenderer renderer;
    RequestHelper* requestHelper;
    json inGameData;
    bool isDay;
    bool firstRender;
    Uint32 lastDayChangeTime;
    const Uint32 DAY_CHANGE_INTERVAL = 1000;
    bool isChatInputFocused;// To keep track of whether the chat input form is focused or not
    std::string chatInputText;       // To store the text entered by the player in the chat input form
    bool hasActed;

};

#endif // INGAMESTATE_H
