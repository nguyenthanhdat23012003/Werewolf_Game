#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "SDLRenderer.h"
#include "../RequestHelper.h"
#include <vector>
#include <string>

class InGameState {
public:
    InGameState(SDL_Window* window, TTF_Font* font, RequestHelper* helper);

    void handleEvents(SDL_Event& e);
    void update();
    void render();

private:
    SDLRenderer renderer;
    RequestHelper* requestHelper;
    std::vector<std::vector<std::string>> playersData;
    std::string currentTimeData;
};

#endif // INGAMESTATE_H
