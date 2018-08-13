#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "GameException.h"



class Game
{
    public:
        static Game* instance;

        static Game* createInstance()
        {
            if(instance == NULL)
            {
                instance = new Game();
                return instance;
            }
            return NULL;
        }

        SDL_Renderer * const getRenderer()  {return renderer;}
        SDL_Window * const getWindow() {return window;}

        ~Game();

    protected:

    private:

        Game();
        const int winWidth, winHeight;
        SDL_Renderer *renderer;
        SDL_Window *window;

};

#endif // GAME_H
