#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "SDLRenderer.h"
#include <vector>
#include <string>

class InGameState {
public:
    InGameState(SDL_Window* window, TTF_Font* font);

    void handleEvents(SDL_Event& e);
    void update();
    void render();

private:
    SDLRenderer renderer;
    std::vector<std::vector<std::string>> playersData;
    std::string currentTimeData;
};

#endif // INGAMESTATE_H
