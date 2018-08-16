#ifndef BOARDTEXTURES_H
#define BOARDTEXTURES_H
#include "Game.h"
#include "Board.h"

class BoardTextures
{
    public:
        BoardTextures(SDL_Renderer *const renderer, int boardSize);
        ~BoardTextures();
        void render(const Board& board);
        void renderBlankBoard();
        int getStoneTexSize() {return stoneTexSize;}

    protected:

    private:
        SDL_Renderer *const renderer;
        SDL_Surface *boardSurface;
        SDL_Texture *boardTexture;
        SDL_Surface *bStoneSurface;
        SDL_Texture *bStoneTexture;
        SDL_Surface *wStoneSurface;
        SDL_Texture *wStoneTexture;
        int boardTexSize;
        int stoneTexSize;
        std::vector<Coordinate> topLeftCorners;
};

#endif // BOARDTEXTURES_H
