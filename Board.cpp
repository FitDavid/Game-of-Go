#include "Board.h"

/// \brief
///
/// \param
/// \param
/// \return
///
///



Board::Board():
    boardSize(19),
    points1D(NULL),
    board(NULL),
    turn(BLACK),
    mouseButtonDown(false),
    bPrisoner(0),
    wPrisoner(0)
{
    points1D = new Point[boardSize*boardSize];
    board = new Point*[boardSize];
    for(int i = 0; i < boardSize; i++)
        board[i] = points1D + i*boardSize;
    newBoard();
    path.reserve(20);
}

Board::~Board()
{
    delete[] board;
    delete[] points1D;
}

/// \brief load a board position from binary file \todo could handle .sgf as well
/// \return true if succeeded
///

bool Board::loadBoard()
{

}

/// \brief Sets every point to EMPTY clearing the board
///


void Board::newBoard()
{
    int i, j;
    for(i = 0, j = 0; i < boardSize; i++)
    {
        for(j = 0; j < boardSize; j++)
        {
            board[i][j] = EMPTY;
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
                    Coordinate pos(x / stoneTexSize, y / stoneTexSize);
                    //if mouse is still in the square
                    if(pos == lastButtonDown)
                        {
                            if(putStone(pos)) turn = oppositeTurn();
                        }
                }
            }
            break;
    }
}

bool Board::putStone(Coordinate pos)
{
    bool success = false;
    if(board[pos.col][pos.row]== EMPTY)
    {
        board[pos.col][pos.row]= turn;

        Coordinate posR(pos.col +1, pos.row);
        if(isInRange(posR))
        {
            if(board[posR.col][posR.row]== oppositeTurn())
            {
                path.resize(0);
                if(isDead(posR, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(posR);
                }
            }
            if(board[posR.col][posR.row]== EMPTY)
                success = true;
        }

        Coordinate posL(pos.col - 1, pos.row);
        if(isInRange(posL))
        {
            if(board[posL.col][posL.row]== oppositeTurn())
            {
                path.resize(0);
                if(isDead(posL, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(posL);
                }
            }
            if(board[posL.col][posL.row]== EMPTY)
                success = true;
        }

        Coordinate posD(pos.col, pos.row + 1);
        if(isInRange(posD))
        {
            if(board[posD.col][posD.row]== oppositeTurn())
            {
                path.resize(0);
                if(isDead(posD, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(posD);
                }
            }
            if(board[posD.col][posD.row]== EMPTY)
                success = true;
        }

        Coordinate posU(pos.col, pos.row - 1);
        if(isInRange(posU))
        {
            if(board[posU.col][posU.row]== oppositeTurn())
            {
                path.resize(0);
                if(isDead(posU, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(posU);
                }
            }
            if(board[posU.col][posU.row]== EMPTY)
                success = true;
        }

        if(success) return true;
        path.resize(0);
        if(isDead(pos, false))
        {
            board[pos.col][pos.row]= EMPTY;
        }
        else return true;
    }
    return false;
}

bool Board::isDead(Coordinate pos,const bool opposite)
{
    path.push_back(pos);
    bool dead = true;
    Point color;
    if(opposite)color = oppositeTurn();
    else color = turn;

    Coordinate posR(pos.col +1, pos.row);
    if(isInRange(posR) && dead)
    {
        if(board[posR.col][posR.row]== EMPTY)
            dead = false;
        else if(board[posR.col][posR.row]== color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posR));
            if(haventBeen)
                dead = isDead(posR, opposite);
        }
    }

    Coordinate posL(pos.col - 1, pos.row);
    if(isInRange(posL) && dead)
    {
        if(board[posL.col][posL.row]== EMPTY)
            dead = false;
        else if(board[posL.col][posL.row]== color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posL));
            if(haventBeen)
                dead = isDead(posL, opposite);
        }
    }

    Coordinate posD(pos.col, pos.row + 1);
    if(isInRange(posD) && dead)
    {
        if(board[posD.col][posD.row]== EMPTY)
            dead = false;
        else if(board[posD.col][posD.row]== color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posD));
            if(haventBeen)
                dead = isDead(posD, opposite);
        }
    }

    Coordinate posU(pos.col, pos.row - 1);
    if(isInRange(posU) && dead)
    {
        if(board[posU.col][posU.row]== EMPTY)
            dead = false;
        else if(board[posU.col][posU.row]== color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posU));
            if(haventBeen)
                dead = isDead(posU, opposite);
        }
    }
    return dead;
}

void Board::removeDeadGroup(Coordinate pos)
{
    path.push_back(pos);
    Point color = board[pos.col][pos.row];
    if(color == EMPTY) throw GameException("Cannot remove from empty point.");
    board[pos.col][pos.row]= EMPTY;
    if(color == BLACK) bPrisoner++;
    if(color == WHITE) wPrisoner++;

    Coordinate posR(pos.col +1, pos.row);
    if(isInRange(posR))
    {
        bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posR));
        if(haventBeen)
            if(board[posR.col][posR.row]== color)
                removeDeadGroup(posR);
    }

    Coordinate posL(pos.col - 1, pos.row);
    if(isInRange(posL))
    {
        bool haventBeen = (path.end()== std::find(path.begin(), path.end(), posL));
        if(haventBeen)
            if(board[posL.col][posL.row]== color)
                removeDeadGroup(posL);
    }

    Coordinate posD(pos.col, pos.row + 1);
    if(isInRange(posD))
    {
        bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posD));
        if(haventBeen)
            if(board[posD.col][posD.row]== color)
                removeDeadGroup(posD);
    }

    Coordinate posU(pos.col, pos.row - 1);
    if(isInRange(posU))
    {
        bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posU));
        if(haventBeen)
            if(board[posU.col][posU.row]== color)
                removeDeadGroup(posU);
    }
}





