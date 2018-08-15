#ifndef BOARD_H
#define BOARD_H
#include "Game.h"

class Board
{
    public:

        typedef struct _position {int col; int row; } Position;

        Board();

        ~Board();

        void loadBoard();

        void newBoard();

        void handleEvents(const SDL_Event& event, int stoneTexSize);

        bool isInRange(int col, int row = 0)
            {
                if(col >= 0 && col < boardSize && row >= 0 && row < boardSize)
                    return true;
                return false;
            }

        bool isLegal(Position pos);

        bool isGonnaDie(Position pos);

        unsigned getBoardSize() const { return boardSize; }

        const Point *const *const getBoard() const {return board;}

        Stone getTurn() const {return turn;}

    protected:

    private:
        unsigned boardSize;
        Point *points1D;
        Point **board;
        Stone turn;
        Position lastButtonDown;


};

#endif // BOARD_H
