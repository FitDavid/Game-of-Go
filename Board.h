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

        bool isInRange(Coordinate coord)///< Overloaded for convenience
            {
                if(coord.col >= 0 && coord.col < boardSize && coord.row >= 0 && coord.row < boardSize)
                    return true;
                return false;
            }

        bool putStone(Coordinate);

        void removeDeadGroup(Coordinate);

        bool isDead(Coordinate, bool opposite);

        Point oppositeTurn() ///< simply returns the opposite color
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
        Point *points1D; ///< this will point to an array holds all the points
        Point **board; ///< this will point to an array holding pointers creating a dynamic 2D array
        Point turn; ///< Holds which player is next
        Coordinate lastButtonDown; ///< Holds where was the mouse button held down last
        bool mouseButtonDown; ///< was it pressed down before?
        std::vector<Coordinate> path; ///< Recursive functions need this to track which points have been already observed.
        unsigned bPrisoner;
        unsigned wPrisoner;
};

#endif // BOARD_H
