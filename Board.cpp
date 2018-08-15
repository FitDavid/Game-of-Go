#include "Board.h"

Board::Board():
    boardSize(19),
    points1D(NULL),
    board(NULL),
    turn(BLACK),
    mouse(UP)
{
    points1D = new Point[boardSize*boardSize];
    board = new Point*[boardSize];
    for(unsigned i = 0; i < boardSize; i++)
        board[i] = points1D + i*boardSize;
    newBoard();
}

Board::~Board()
{
    delete[] board;
    delete[] points1D;

}

void Board::newBoard()
{
    unsigned i, j;
    for(i = 0, j = 0; i < boardSize; i++)
    {
        for(j = 0; j < boardSize; j++)
        {
            board[i][j].x = i*25;
            board[i][j].y = j*25;
            board[i][j].stone = EMPTY;
        }
    }

}

void Board::handleEvents(const SDL_Event& event)
{

    switch(event.type)
    {
        case SDL_MOUSEMOTION:
            {
                int col = event.motion.x / boardSize;
                int row = event.motion.y / boardSize;
                if(isInRange(col, row) && board[col][row].stone == EMPTY)
                    ;
            }
    }
}


