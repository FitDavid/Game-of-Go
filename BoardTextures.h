#ifndef BOARDTEXTURES_H
#define BOARDTEXTURES_H
#include "Game.h"


class BoardTextures
{
    typedef enum _mouse{UP,DOWN} MouseButton;

    public:
        BoardTextures(SDL_Renderer *const renderer);
        ~BoardTextures();
        void render();

    protected:

    private:
        SDL_Renderer *const renderer;
        MouseButton mouse;
        SDL_Surface *boardSurface;
        SDL_Texture *boardTexture;
        SDL_Surface *bStoneSurface;
        SDL_Texture *bStoneTexture;
        SDL_Surface *wStoneSurface;
        SDL_Texture *wStoneTexture;
        int boardTexSize;
        int stoneTexSize;
};

#endif // BOARDTEXTURES_H
