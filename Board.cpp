#include "Board.h"

Board::Board():
    boardSize(19),
    points1D(NULL),
    board(NULL),
    turn(BLACK),
    mouseButtonDown(false)
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
                        mouseButtonDown = true;
                        lastButtonDown.col = col;
                        lastButtonDown.row = row;
                    }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            {
                if(mouseButtonDown)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    int col = x / stoneTexSize;
                    int row = y / stoneTexSize;
                    Position pos = {col, row};
                    //if mouse is still in the square
                    if(pos.col == lastButtonDown.col && pos.row == lastButtonDown.row)
                        putStone(pos);
                }
            }
            break;
    }
}

void Board::putStone(Position pos)
{
    if(board[pos.col][pos.row].stone == EMPTY && isLegal(pos))
    {
        board[pos.col][pos.row].stone = turn;
        if(turn == BLACK) turn = WHITE;
        else if(turn == WHITE) turn = BLACK;
    }
}

bool Board::isLegal(Position pos)
{
    if(board[pos.col][pos.row].stone == EMPTY)
    {
        Position posR = {pos.col +1, pos.row};
        if(isInRange(posR))
        {
            if(board[posR.col][posR.row].stone == EMPTY)
                return true;
            else if(isGonnaDie(posR))
                return true;
        }
        Position posL = {pos.col - 1, pos.row};
        if(isInRange(posL))
        {
            if(board[posL.col][posL.row].stone == EMPTY)
                return true;
            else if(isGonnaDie(posL))
                return true;
        }
        Position posD = {pos.col, pos.row + 1};
        if(isInRange(posD))
        {
            if(board[posD.col][posD.row].stone == EMPTY)
                return true;
            else if(isGonnaDie(posD))
                return true;
        }
        Position posU = {pos.col, pos.row - 1};
        if(isInRange(posU))
        {
            if(board[posU.col][posU.row].stone == EMPTY)
                return true;
            else if(isGonnaDie(posU))
                return true;
        }
    }
    return false;
}

bool Board::isGonnaDie(Position pos) //should throw exception if board is in illegal state
{
    return true;
}


