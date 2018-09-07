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
        SDL_Renderer *const renderer; ///< the rendering context created by Game class, and passed to the constructor
        SDL_Surface *boardSurface; // The board is going to be 19*25 == 475 pixels wide
        SDL_Texture *boardTexture;
        SDL_Surface *bStoneSurface; // The stones are going to be 25 pixels wide. This makes them easier to render to the right intersection.
        SDL_Texture *bStoneTexture;
        SDL_Surface *wStoneSurface;
        SDL_Texture *wStoneTexture;
        int boardTexSize;
        int stoneTexSize;
        std::vector<Coordinate> topLeftCorners;
        /// This holds the top left corners of the square where the stones need to be rendered.
        /// The top left corner of a square is between intersections.
        // I tried 2D std::vector but that was too complicated
};

#endif // BOARDTEXTURES_H
