#ifndef BOARD_H
#define BOARD_H
#include "Game.h"

class Board
{
    public:

        typedef enum _mouse{UP,DOWN} MouseButton;

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

        unsigned getBoardSize() const { return boardSize; }

        const Point *const *const getBoard() const {return board;}

        Stone getTurn() const {return turn;}

    protected:

    private:
        unsigned boardSize;
        Point *points1D;
        Point **board;
        Stone turn;
        MouseButton mouse;


};

#endif // BOARD_H
