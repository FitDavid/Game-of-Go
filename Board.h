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

        bool isInRange(int col, int row = 0)
            {
                if(col >= 0 && col < boardSize && row >= 0 && row < boardSize)
                    return true;
                return false;
            }

        unsigned getBoardSize() { return boardSize; }

        const Point *const *const getBoard() {return board;}

    protected:

    private:
        unsigned boardSize;
        Point *points1D;
        Point **board;
        Stone turn;
        MouseButton mouse;


};

#endif // BOARD_H
