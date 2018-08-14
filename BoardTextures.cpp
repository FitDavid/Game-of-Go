#include "BoardTextures.h"

BoardTextures::BoardTextures(SDL_Renderer *const renderer):
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

    renderBlankBoard();
    SDL_RenderPresent(renderer);
}

BoardTextures::~BoardTextures()
{

    SDL_DestroyTexture(boardTexture);
    SDL_DestroyTexture(bStoneTexture);
    SDL_DestroyTexture(wStoneTexture);

}

void BoardTextures::render(unsigned boardSize)
{
    renderBlankBoard();
    unsigned i, j;
    for(i = 0, j = 0; i < boardSize; i++)
        for(j = 0; j < boardSize; j++)
    {

    }
    SDL_RenderPresent(renderer);
}

void BoardTextures::renderBlankBoard()
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_Rect dst = {0, 0, boardTexSize, boardTexSize};
    SDL_RenderCopy(renderer, boardTexture, NULL, &dst);
}
