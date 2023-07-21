#ifndef LOBBYLISTSTATE_H
#define LOBBYLISTSTATE_H

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

using namespace std;
using json = nlohmann::json;

class LobbyListState {
public:
    LobbyListState(SDL_Window* window, TTF_Font* font, RequestHelper* helper);
    void handleEvents(SDL_Event& e);
    void update();
    void render();

private:
    SDLRenderer renderer;
    RequestHelper* requestHelper;
    json lobbyData;
};

#endif // LOBBYLISTSTATE_H

