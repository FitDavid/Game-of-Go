#ifndef BOARD_H
#define BOARD_H
#include "Game.h"
#include <vector>
#include <algorithm>
//#include <iostream>

/// \brief Contains current position on the board in a 2D array. Checks if move is legal and can put stone if so.
/// \todo making functions const if possible
///

class Board
{
    public:

        Board();

        ~Board();

        void handleEvents(const SDL_Event& event, int stoneTexSize);

        bool isInRange(int col, int row = 0) const
            {
                if(col >= 0 && col < boardSize && row >= 0 && row < boardSize)
                    return true;
                return false;
            }

        bool isInRange(Coordinate coord) const
            {
                if(coord.col >= 0 && coord.col < boardSize && coord.row >= 0 && coord.row < boardSize)
                    return true;
                return false;
            }

        Point getTurn() const {return turn;}

        unsigned getBoardSize() const { return boardSize; }

        const Point *const *const getBoard() const {return board;}

        /*void printBoard(Point ** board)
        {
            int i,j;
            for(i = 0, j = 0; i < 5; i++)
            {
                for(j = 0; j < 5; j++)
                {
                    switch(board[j][i])
                    {
                        case EMPTY: std::cout << "_"; break;
                        case BLACK: std::cout << "@"; break;
                        case WHITE: std::cout << "O"; break;
                    }
                }
                std::cout << std::endl;
            }
        }*/

    private:
        bool loadBoard();

        void newBoard();

        bool putStone(Coordinate);

        void removeDeadGroup(Coordinate);

        bool isDead(Coordinate);

        bool compareBoards();

        void copyBoards();

        Point oppositeTurn()
        {
            if(turn == BLACK) return WHITE;
            else if(turn == WHITE) return BLACK;
            throw GameException("Invalid turn.");
        }

        int boardSize; ///< Can be 9, 13 or 19.
        Point *points1D; ///< These pointers are needed for creating dynamic 2D arrays.
        Point **board; ///< Current board position.
        Point *prevPoints1D; ///< These pointers are needed for creating dynamic 2D arrays.
        Point **prevBoard; ///< The board one move before (nedded for ko)
        Point *prevprevPoints1D; ///< These pointers are needed for creating dynamic 2D arrays.
        Point **prevprevBoard;///< The board two moves before (nedded for ko)
        std::vector<Coordinate> moves; ///< holds the sequence of moves
        Point turn; ///< Holds which player is next
        Coordinate lastButtonDown; ///< Holds where was the mouse button held down last
        bool mouseButtonDown; ///< was it pressed down before?
        std::vector<Coordinate> stonesRemoved; ///< Recursive functions need this to track which points have been visited already. Used by \code removeDeadGroup() \endcode .
        std::vector<Coordinate> stonesObserved; ///< Same as stonesRemoved, but this is used by isDead()
        unsigned bPrisoner; ///< counter for black prisoners
        unsigned wPrisoner; ///< counter for white prisoners
};

#endif // BOARD_H
