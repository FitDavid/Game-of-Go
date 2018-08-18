/// \file Game.h
/// \brief Contains Game singleton and Point enumeration
///

#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "GameException.h"

/// \brief Describes what is on the point (intersection)
///

typedef enum _point {EMPTY, BLACK, WHITE} Point;

/// \brief Holds coordinates of a point on board, but also can contain pixel coordinates.
/// Origin is at the top left corner in either case. Columns numbered from left to right 0-18, rows numbered from top to bottom 0-18.
/// Or from 0 to window size when it holds pixels.
///

class Coordinate
{
public:
    int col;
    int row;
    Coordinate():col(0), row(0) {}
    Coordinate(int c, int r):col(c), row(r) {}
    bool operator==(Coordinate pos)
    {
        return (this->col == pos.col) && (this->row == pos.row);
    }
};


/// \brief Game singleton holds necessary variables/functions for initiating SDL2
///

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

    SDL_Renderer * const getRenderer()
    {
        return renderer;
    }
    SDL_Window * const getWindow()
    {
        return window;
    }

    ~Game();

protected:

private:

    Game();
    const int winWidth, winHeight;
    SDL_Renderer *renderer;
    SDL_Window *window;

};

#endif // GAME_H
