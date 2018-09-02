#include "BoardTextures.h"

/// \brief Loads all the textures from .png
///
/// \param renderer The rendering context provided by Game class
/// \param boardSize diameter of the board 19, 13 or 9
///


BoardTextures::BoardTextures(SDL_Renderer *const renderer, int boardSize):
    renderer(renderer),
    boardSurface(NULL),
    boardTexture(NULL),
    bStoneSurface(NULL),
    bStoneTexture(NULL),
    wStoneSurface(NULL),
    wStoneTexture(NULL),
    boardTexSize(0),
    stoneTexSize(0)
{
    boardSurface = IMG_Load("goBoard.png");
    if(boardSurface == NULL) throw GameException("Board image loading error.");
    boardTexture = SDL_CreateTextureFromSurface(renderer, boardSurface);
    boardTexSize = boardSurface->w;
    SDL_FreeSurface(boardSurface);
    boardSurface = NULL;

    bStoneSurface = IMG_Load("blackstone.png");
    if(bStoneSurface == NULL) throw GameException("Black stone image loading error.");
    bStoneTexture = SDL_CreateTextureFromSurface(renderer, bStoneSurface);
    stoneTexSize = bStoneSurface->w;
    SDL_FreeSurface(bStoneSurface);
    bStoneSurface = NULL;

    wStoneSurface = IMG_Load("whitestone.png");
    if(wStoneSurface == NULL) throw GameException("White stone image loading error.");
    wStoneTexture = SDL_CreateTextureFromSurface(renderer, wStoneSurface);
    SDL_FreeSurface(wStoneSurface);
    wStoneSurface = NULL;

    SDL_SetTextureBlendMode(boardTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(bStoneTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(wStoneTexture, SDL_BLENDMODE_BLEND);

    topLeftCorners.reserve(boardSize*boardSize);
    int i,j;
    for(i = 0, j = 0; i < boardSize; i++)
    {
        for(j = 0; j < boardSize; j++)
        {
            topLeftCorners[i*boardSize + j].col = i*stoneTexSize;
            topLeftCorners[i*boardSize + j].row = j*stoneTexSize;
        }
    }

    renderBlankBoard();
    SDL_RenderPresent(renderer);
    return;
}

BoardTextures::~BoardTextures()
{

    SDL_DestroyTexture(boardTexture);
    SDL_DestroyTexture(bStoneTexture);
    SDL_DestroyTexture(wStoneTexture);
}

/// \brief Renders textures according to boardposition.
/// Also, it renders a transparent stone of next player's color to where the mouse points
///

void BoardTextures::render(const Board& board)
{
    renderBlankBoard();
    SDL_Rect dst;
    dst.w = dst.h = stoneTexSize;
    const Point * const * const boardPointer = board.getBoard();
    unsigned i, j;
    for(i = 0, j = 0; i < board.getBoardSize(); i++)
        for(j = 0; j < board.getBoardSize(); j++)
    {
        if(boardPointer[i][j] != EMPTY)
        {
            dst.x = topLeftCorners[i*board.getBoardSize() + j].col;
            dst.y = topLeftCorners[i*board.getBoardSize() + j].row;
            if(boardPointer[i][j] == BLACK)
            {
                SDL_RenderCopy(renderer, bStoneTexture, NULL, &dst);
            }
            else if(boardPointer[i][j] == WHITE)
            {
                SDL_RenderCopy(renderer, wStoneTexture, NULL, &dst);
            }
        }
    }
    //Rendering mouse pointer
    int x, y;
    SDL_GetMouseState(&x, &y);
    int col = x / stoneTexSize;
    int row = y / stoneTexSize;
    if(board.isInRange(row, col))
    {
        dst.x = col * stoneTexSize; // we cut off the part over N*25, to get the top left corner of a square over an intersection
        dst.y = row * stoneTexSize;
        if(board.getTurn() == BLACK)
        {
            SDL_SetTextureAlphaMod(bStoneTexture, 0x80); // make it 50% transparent
            SDL_RenderCopy(renderer, bStoneTexture, NULL, &dst);
            SDL_SetTextureAlphaMod(bStoneTexture, 0xFF);
        }
        else if(board.getTurn() == WHITE)
        {
            SDL_SetTextureAlphaMod(wStoneTexture, 0x80);
            SDL_RenderCopy(renderer, wStoneTexture, NULL, &dst);
            SDL_SetTextureAlphaMod(wStoneTexture, 0xFF);
        }
    }
    SDL_RenderPresent(renderer);
}

/// \brief Renders a board on screen without any stones.
///

void BoardTextures::renderBlankBoard()
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_Rect dst = {0, 0, boardTexSize, boardTexSize};
    SDL_RenderCopy(renderer, boardTexture, NULL, &dst);
}

