#ifndef INLOBBYSTATE_H
#define INLOBBYSTATE_H

#include <iostream>
#include <vector>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "../json-develop/single_include/nlohmann/json.hpp"
#include "../RequestHelper.h"
#include "../Message.h"
#include "SDLRenderer.h"
#include "GameState.h"

using json = nlohmann::json;

class InLobbyState {
public:
    InLobbyState(SDL_Window* window, TTF_Font* font, RequestHelper* helper);
    void handleEvents(SDL_Event& e);
    void update();
    void render();

private:
    SDLRenderer renderer;
    json inLobbyData;
    RequestHelper* requestHelper;
    bool firstRender;
    Uint32 lastUpdateTime;
    const Uint32 UPDATE_INTERVAL = 1000;
};

#endif // INLOBBYSTATE_H
