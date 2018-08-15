#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "GameException.h"

typedef enum _stonetype {EMPTY, BLACK, WHITE} Stone;

typedef struct _point
        {
            int x;
            int y;
            Stone stone;
        } Point;

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
