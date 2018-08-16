#ifndef BOARD_H
#define BOARD_H
#include "Game.h"
#include <vector>
#include <algorithm>

/// \brief Contains current position on the board in a 2D array. Checks if move is legal and can put stone if so.
///

class Board
{
    public:

        Board();

        ~Board();

        bool loadBoard();

        void newBoard();

        void handleEvents(const SDL_Event& event, int stoneTexSize);

        bool isInRange(int col, int row = 0)///< Checks if given Coordinates are on the board.
            {
                if(col >= 0 && col < boardSize && row >= 0 && row < boardSize)
                    return true;
                return false;
            }

        bool isInRange(Coordinate pos)///< Overloaded for convenience
            {
                if(pos.col >= 0 && pos.col < boardSize && pos.row >= 0 && pos.row < boardSize)
                    return true;
                return false;
            }

        bool putStone(Coordinate pos);

        void removeDeadGroup(Coordinate pos);

        bool isDead(Coordinate pos, bool opposite);

        Point oppositeTurn()
        {
            if(turn == BLACK) return WHITE;
            else if(turn == WHITE) return BLACK;
            throw GameException("Invalid turn.");
        }

        Point getTurn() const {return turn;}

        unsigned getBoardSize() const { return boardSize; }

        const Point *const *const getBoard() const {return board;}

    protected:

    private:
        int boardSize;
        Point *points1D;
        Point **board;
        Point turn; ///< Holds which player is next
        Coordinate lastButtonDown; ///< Holds where was the mouse button held down last
        bool mouseButtonDown;
        std::vector<Coordinate> path;
        unsigned bPrisoner;
        unsigned wPrisoner;
};

#endif // BOARD_H
