#ifndef LOBBYLISTSTATE_H
#define LOBBYLISTSTATE_H

#include <vector>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDLRenderer.h"
#include "GameState.h"

class LobbyListState {
public:
    LobbyListState(SDL_Window* window, TTF_Font* font);
    void handleEvents(SDL_Event& e);
    void update();
    void render();

private:
    SDLRenderer renderer;
    std::vector<std::vector<std::string>> lobbyData;
};

#endif // LOBBYLISTSTATE_H

