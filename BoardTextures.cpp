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
    return;
}

BoardTextures::~BoardTextures()
{

    SDL_DestroyTexture(boardTexture);
    SDL_DestroyTexture(bStoneTexture);
    SDL_DestroyTexture(wStoneTexture);

}

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
        if(boardPointer[i][j].stone != EMPTY)
            dst.x = boardPointer[i][j].x;
            dst.y = boardPointer[i][j].y;
            if(boardPointer[i][j].stone == BLACK)
            {
                SDL_RenderCopy(renderer, bStoneTexture, NULL, &dst);
            }
            else if(boardPointer[i][j].stone == WHITE)
            {
                SDL_RenderCopy(renderer, wStoneTexture, NULL, &dst);
            }
    }
    //Rendering mouse pointer
    int x, y;
    SDL_GetMouseState(&x, &y);
    dst.x = x / stoneTexSize * stoneTexSize;
    dst.y = y / stoneTexSize * stoneTexSize;
    if(board.getTurn() == BLACK)
        {
            SDL_SetTextureAlphaMod(bStoneTexture, 0x80);
            SDL_RenderCopy(renderer, bStoneTexture, NULL, &dst);
            SDL_SetTextureAlphaMod(bStoneTexture, 0xFF);
        }
    else if(board.getTurn() == WHITE)
        {
            SDL_SetTextureAlphaMod(wStoneTexture, 0x80);
            SDL_RenderCopy(renderer, wStoneTexture, NULL, &dst);
            SDL_SetTextureAlphaMod(wStoneTexture, 0xFF);
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

