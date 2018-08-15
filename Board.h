#ifndef BOARD_H
#define BOARD_H
#include "Game.h"
#include <vector>
#include <algorithm>

class Board
{
    public:

        class Position
        {
        public:
            int col;
            int row;
            Position():col(0), row(0) {}
            Position(int c, int r):col(c), row(r) {}
            bool operator==(Position pos) {return (this->col == pos.col) && (this->row == pos.row);}
        };

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

        bool isInRange(Position pos)
            {
                if(pos.col >= 0 && pos.col < boardSize && pos.row >= 0 && pos.row < boardSize)
                    return true;
                return false;
            }

        bool putStone(Position pos);

        void removeDeadGroup(Position pos);

        bool isDead(Position pos, bool opposite);

        Stone oppositeTurn()
        {
            if(turn == BLACK) return WHITE;
            else if(turn == WHITE) return BLACK;
            throw GameException("Invalid turn.");
        }

        Stone getTurn() const {return turn;}

        unsigned getBoardSize() const { return boardSize; }

        const Point *const *const getBoard() const {return board;}

    protected:

    private:
        int boardSize;
        Point *points1D;
        Point **board;
        Stone turn;
        Position lastButtonDown;
        bool mouseButtonDown;
        std::vector<Position> path;
        unsigned bPrisoner;
        unsigned wPrisoner;
};

#endif // BOARD_H
