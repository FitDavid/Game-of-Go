#ifndef BOARD_H
#define BOARD_H
#include "Game.h"

class Board
{
    public:

        typedef enum _mouse{UP,DOWN} MouseButton;

        typedef enum _stonetype {EMPTY, BLACK, WHITE} Stone;

        typedef struct _point
        {
            int x; int y;
            Stone stone;
            bool isTransparent;
        } Point;

        Board();

        ~Board();

        void loadBoard();

        void newBoard();

        void handleEvents(const SDL_Event& event);



    protected:

    private:
        int boardSize;
        Point *point1D;
        Point **point;
        Stone turn;
        MouseButton mouse;


};

#endif // BOARD_H
