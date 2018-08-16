#include "Game.h"

Game* Game::instance = NULL;

Game::Game():winWidth(640), winHeight(480), renderer(NULL), window(NULL) ///< \brief Initializes SDL, and other extensions
{
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO ) != 0) throw GameException("SDL Initialization failure");
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) throw GameException("SDL_image initialization failure.");
    if(TTF_Init() != 0) throw GameException("SDL_TTF initialization failure.");
    window = SDL_CreateWindow("Game of Go", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    if(window == NULL) throw GameException("Window creation failure.");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) throw GameException("Renderer creation failure.");
}

Game::~Game() ///< \brief Close SDL structures
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}


