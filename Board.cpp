#include "Board.h"

Board::Board():
    boardSize(19),
    points1D(NULL),
    board(NULL),
    turn(BLACK)
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

void Board::handleEvents(const SDL_Event& event, int stoneTexSize)
{

    switch(event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int col = x / stoneTexSize;
                int row = y / stoneTexSize;
                if(isInRange(col, row))
                    {
                        lastButtonDown.col = col;
                        lastButtonDown.row = row;
                    }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int col = x / stoneTexSize;
                int row = y / stoneTexSize;
                Position pos = {col, row};
                //if square is empty and mouse is still in the square and move is legal
                if(board[col][row].stone == EMPTY &&
                   pos.col == lastButtonDown.col &&
                   pos.row == lastButtonDown.row &&
                   isLegal(pos))
                {
                    board[col][row].stone = turn;
                    if(turn == BLACK) turn = WHITE;
                    else if(turn == WHITE) turn = BLACK;
                }
            }
    }
}

bool Board::isLegal(Position pos)
{

    return true;
}

bool Board::isGonnaDie(Position pos) //should throw ecseption if board is in illegal state
{
    return true;
}


