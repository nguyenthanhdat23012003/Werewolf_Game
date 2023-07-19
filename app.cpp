#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

SDL_Rect* drawRect(int x, int y, int w, int h, int r, int g, int b);
SDL_Surface* loadAndStretchImage(const char* imagePath, int newWidth, int newHeight);
void blitSurfaceAtLocation(SDL_Surface* surface, int x, int y);
void drawTable(const vector<vector<string>>& tableData);
void drawText(const string& text, int x, int y, Uint8 r, Uint8 g, Uint8 b);

// Helper function to check if the given coordinates are within a rectangle
bool isPointInRect(int x, int y, const SDL_Rect& rect) {
    return (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h);
}

enum class GameState {
    LOBBY_LIST,
    IN_LOBBY
};

// Lobby data (id, status)
vector<vector<string>> lobbyData = {
        { "1", "Waiting" },
        { "2", "In Game" },
        // Add more lobby data as needed
};

// Players data (id, status)
vector<vector<string>> playersData = {
        { "1", "Ready" },
        { "2", "Not Ready" },
        // Add more players data as needed
};

GameState currentState = GameState::LOBBY_LIST;

int main(int argc, char* argv[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        // Load media
        loadMedia();

        bool quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;

                    if (currentState == GameState::LOBBY_LIST) {
                        // Check for join button clicks in the lobby list
                        for (int i = 0; i < lobbyData.size(); ++i) {
                            const SDL_Rect joinButtonRect = { 600, 100 + i * 50, 100, 30 };
                            if (isPointInRect(mouseX, mouseY, joinButtonRect)) {
                                currentState = GameState::IN_LOBBY;
                                break;
                            }
                        }
                    }
                }
            }

            // Clear the screen
            SDL_FillRect(gScreenSurface, nullptr, 0);

            // Draw the current state
            if (currentState == GameState::LOBBY_LIST) {
                drawTable(lobbyData);
            } else if (currentState == GameState::IN_LOBBY) {
                drawTable(playersData);
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
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
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


SDL_Rect *drawRect(int x, int y, int w, int h, int r, int g, int b) {
    SDL_Rect *rect = new SDL_Rect{x, y, w, h};
    Uint32 rectColor = SDL_MapRGB(gScreenSurface->format, r, g, b);
    SDL_FillRect(gScreenSurface, rect, rectColor);
    return rect;
}

SDL_Surface *loadAndStretchImage(const char *imagePath, int newWidth, int newHeight) {
    // Load the image
    SDL_Surface *loadedImage = SDL_LoadBMP(imagePath);
    if (loadedImage == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", imagePath, SDL_GetError());
        return NULL;
    }

    // Create a new surface with the desired width and height
    SDL_Surface *stretchedImage = SDL_CreateRGBSurfaceWithFormat(0, newWidth, newHeight, 32,
                                                                 loadedImage->format->format);
    if (stretchedImage == NULL) {
        printf("Unable to create stretched surface! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(loadedImage);
        return NULL;
    }

    // Stretch the loaded image to the desired size
    if (SDL_BlitScaled(loadedImage, NULL, stretchedImage, NULL) != 0) {
        printf("Unable to stretch image! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(loadedImage);
        SDL_FreeSurface(stretchedImage);
        return NULL;
    }

    // Free the original loaded image
    SDL_FreeSurface(loadedImage);

    return stretchedImage;
}

void blitSurfaceAtLocation(SDL_Surface *surface, int x, int y) {
    // Create a named SDL_Rect object
    SDL_Rect destinationRect = {x, y, 0, 0};

    if (SDL_BlitSurface(surface, NULL, gScreenSurface, &destinationRect) != 0) {
        printf("Unable to blit surface! SDL Error: %s\n", SDL_GetError());
    }
}

void drawTable(const vector<vector<string>>& tableData) {
    // Draw table header
    drawText("ID", 200, 50, 255, 255, 255);
    drawText("Status", 400, 50, 255, 255, 255);

    // Draw table data
    for (int i = 0; i < tableData.size(); ++i) {
        drawText(tableData[i][0], 200, 100 + i * 50, 255, 255, 255);
        drawText(tableData[i][1], 400, 100 + i * 50, 255, 255, 255);
        drawRect(600, 100 + i * 50, 100, 30, 255, 255, 255);
    }
}

void drawText(const string& text, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    SDL_Color textColor = { r, g, b };
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
    if (textSurface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Rect destinationRect = { x, y, textSurface->w, textSurface->h };
    SDL_BlitSurface(textSurface, nullptr, gScreenSurface, &destinationRect);

    SDL_FreeSurface(textSurface);
}
