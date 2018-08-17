#ifndef BOARDTEXTURES_H
#define BOARDTEXTURES_H
#include "Game.h"
#include "Board.h"

/// \brief Loads and stores textures needed for drawing the board.
///

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
        SDL_Renderer *const renderer; ///< renderer given by Game class
        SDL_Surface *boardSurface;
        SDL_Texture *boardTexture;
        SDL_Surface *bStoneSurface;
        SDL_Texture *bStoneTexture;
        SDL_Surface *wStoneSurface;
        SDL_Texture *wStoneTexture;
        int boardTexSize;
        int stoneTexSize;
        std::vector<Coordinate> topLeftCorners;
        /// this holds the top left corners of the square where the stones need to be rendered
        /// this is exactly between intersections
};

#endif // BOARDTEXTURES_H
