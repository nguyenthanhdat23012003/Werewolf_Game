#include <iostream>
#include "SDLRenderer.h"

SDLRenderer::SDLRenderer(SDL_Window* window, TTF_Font* font) : mWindow(window), mFont(font) {
    mScreenSurface = SDL_GetWindowSurface(mWindow);
}

SDL_Window* SDLRenderer::getWindow() {
    return mWindow;
}

SDL_Surface* SDLRenderer::getScreenSurface() {
    return mScreenSurface;
}

TTF_Font* SDLRenderer::getFont() {
    return mFont;
}

void SDLRenderer::drawText(const std::string& text, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    if (text.empty()) return;
    SDL_Color textColor = { r, g, b, 1 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text.c_str(), textColor);
    if (textSurface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Rect destinationRect = { x, y, textSurface->w, textSurface->h };
    SDL_BlitSurface(textSurface, nullptr, mScreenSurface, &destinationRect);

    SDL_FreeSurface(textSurface);
}

void SDLRenderer::drawRect(int x, int y, int w, int h, int r, int g, int b) {
    SDL_Rect rect = { x, y, w, h };
    Uint32 rectColor = SDL_MapRGB(mScreenSurface->format, r, g, b);
    SDL_FillRect(mScreenSurface, &rect, rectColor);
}

SDL_Surface* SDLRenderer::loadAndStretchImage(const char* imagePath, int newWidth, int newHeight) {
    // Load the image
    SDL_Surface* loadedImage = SDL_LoadBMP(imagePath);
    if (loadedImage == nullptr) {
        printf("Unable to load image %s! SDL Error: %s\n", imagePath, SDL_GetError());
        return nullptr;
    }

    // Create a new surface with the desired width and height
    SDL_Surface* stretchedImage = SDL_CreateRGBSurfaceWithFormat(0, newWidth, newHeight, 32, loadedImage->format->format);
    if (stretchedImage == nullptr) {
        printf("Unable to create stretched surface! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(loadedImage);
        return nullptr;
    }

    // Stretch the loaded image to the desired size
    if (SDL_BlitScaled(loadedImage, nullptr, stretchedImage, nullptr) != 0) {
        printf("Unable to stretch image! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(loadedImage);
        SDL_FreeSurface(stretchedImage);
        return nullptr;
    }

    // Free the original loaded image
    SDL_FreeSurface(loadedImage);

    return stretchedImage;
}

void SDLRenderer::blitSurfaceAtLocation(SDL_Surface* surface, int x, int y) {
    SDL_Rect destinationRect = { x, y, 0, 0 };

    if (SDL_BlitSurface(surface, nullptr, mScreenSurface, &destinationRect) != 0) {
        printf("Unable to blit surface! SDL Error: %s\n", SDL_GetError());
    }
}

bool SDLRenderer::isPointInRect(int x, int y, const SDL_Rect& rect) {
    return (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h);
}

void SDLRenderer::updateWindowSurface() {
    SDL_UpdateWindowSurface(mWindow);
}
