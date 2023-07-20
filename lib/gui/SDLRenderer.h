#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

class SDLRenderer {
public:
    SDLRenderer(SDL_Window* window, TTF_Font* font);
    SDL_Window* getWindow();
    SDL_Surface* getScreenSurface();
    TTF_Font* getFont();
    void drawText(const std::string& text, int x, int y, Uint8 r, Uint8 g, Uint8 b);
    void drawRect(int x, int y, int w, int h, int r, int g, int b);
    SDL_Surface* loadAndStretchImage(const char* imagePath, int newWidth, int newHeight);
    void blitSurfaceAtLocation(SDL_Surface* surface, int x, int y);
    void updateWindowSurface();
    bool isPointInRect(int x, int y, const SDL_Rect& rect);

private:
    SDL_Window* mWindow;
    SDL_Surface* mScreenSurface;
    TTF_Font* mFont;
};

#endif // SDL_RENDERER_H
