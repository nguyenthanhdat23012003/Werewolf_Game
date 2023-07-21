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

    void handleEvents(SDL_Event& e);
    void update();
    void render();

private:
    SDLRenderer renderer;
    RequestHelper* requestHelper;
    json inGameData;
    bool isDay;
    bool firstRender;
};

#endif // INGAMESTATE_H
