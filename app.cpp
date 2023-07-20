#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "lib/gui/GameState.h"
#include "lib/gui/LobbyListState.h"
#include "lib/gui/InLobbyState.h"

using namespace std;

// Constants for window dimensions
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

bool init();

void loadMedia();

void close();

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
TTF_Font* gFont = nullptr;

int main(int argc, char* argv[]) {
    if (!init()) {
        cout << "Failed to initialize!\n";
    }
    else {
        loadMedia();

        bool quit = false;
        SDL_Event e;

        // Create game states objects
        LobbyListState lobbyListState(gWindow, gFont);
        InLobbyState inLobbyState(gWindow, gFont);

        while (!quit) {
            GameState::State currentState = GameState::getCurrentState();
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                // Handle events based on the current state
                if (currentState == GameState::State::LOBBY_LIST) {
                    lobbyListState.handleEvents(e);
                } else if (currentState == GameState::State::IN_LOBBY) {
                    inLobbyState.handleEvents(e);
                }
            }

            // Update the current state
            if (currentState == GameState::State::LOBBY_LIST) {
                lobbyListState.update();
            } else if (currentState == GameState::State::IN_LOBBY) {
                inLobbyState.update();
            }

            // Clear the screen
            SDL_FillRect(gScreenSurface, nullptr, 0);

            // Draw the current state
            if (currentState == GameState::State::LOBBY_LIST) {
                lobbyListState.render();
            } else if (currentState == GameState::State::IN_LOBBY) {
                inLobbyState.render();
            }

            SDL_UpdateWindowSurface(gWindow);
        }
    }
    close();
    return 0;
}

bool init() {
    bool success = true;

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    } else {
        gFont = TTF_OpenFont("font/Roboto-Regular.ttf", 24);
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        gWindow = SDL_CreateWindow("Werewolf", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        } else {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

void loadMedia() {

}

void close() {
    TTF_CloseFont(gFont);
    gFont = nullptr;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}