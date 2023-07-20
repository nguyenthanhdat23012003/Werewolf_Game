#ifndef INLOBBYSTATE_H
#define INLOBBYSTATE_H

#include <vector>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDLRenderer.h"
#include "GameState.h"

class InLobbyState {
public:
    InLobbyState(SDL_Window* window, TTF_Font* font);
    void handleEvents(SDL_Event& e);
    void update();
    void render();

private:
    SDLRenderer renderer;
    std::vector<std::vector<std::string>> playersData;
};

#endif // INLOBBYSTATE_H
