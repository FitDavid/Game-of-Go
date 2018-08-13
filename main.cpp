#include "Game.h"
#include "Board.h"
#include "Menu.h"
#include "BoardTextures.h"
#include <iostream>


int main(int argc, char *args[])
{

    try
    {
        Game* game = Game::createInstance();
        if(game == NULL) throw GameException("Cannot create game instance.");
        Board board;
        BoardTextures boardTex(game->getRenderer());
        bool quit = false;
        SDL_Event event;
        while(!quit)
        {
            while(SDL_PollEvent(&event) > 0)
            {
                if(event.type == SDL_QUIT) quit = true;
                board.handleEvents(event);

            }
            boardTex.render();
        }
    }
    catch(const std::exception& exc)
    {
        std::cout << exc.what() << std::endl;
        std::cout << "SDL error message: " << SDL_GetError() << std::endl;
        std::cout << "IMG error message: " << IMG_GetError() << std::endl;
        std::cout << "TTF error message: " << TTF_GetError() << std::endl;

    }
    catch(...)
    {
        std::cout <<  "Unknown exception." << std::endl;
    }


}
