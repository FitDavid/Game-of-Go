#include "Board.h"

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

void Board::newBoard()
{
    int i, j;
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
                    Position pos(x / stoneTexSize, y / stoneTexSize);
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

bool Board::putStone(Position pos)
{
    bool success = false;
    if(board[pos.col][pos.row].stone == EMPTY)
    {
        board[pos.col][pos.row].stone = turn;

        Position posR(pos.col +1, pos.row);
        if(isInRange(posR))
        {
            if(board[posR.col][posR.row].stone == oppositeTurn())
            {
                path.resize(0);
                if(isDead(posR, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(posR);
                }
            }
            if(board[posR.col][posR.row].stone == EMPTY)
                success = true;
        }

        Position posL(pos.col - 1, pos.row);
        if(isInRange(posL))
        {
            if(board[posL.col][posL.row].stone == oppositeTurn())
            {
                path.resize(0);
                if(isDead(posL, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(posL);
                }
            }
            if(board[posL.col][posL.row].stone == EMPTY)
                success = true;
        }

        Position posD(pos.col, pos.row + 1);
        if(isInRange(posD))
        {
            if(board[posD.col][posD.row].stone == oppositeTurn())
            {
                path.resize(0);
                if(isDead(posD, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(posD);
                }
            }
            if(board[posD.col][posD.row].stone == EMPTY)
                success = true;
        }

        Position posU(pos.col, pos.row - 1);
        if(isInRange(posU))
        {
            if(board[posU.col][posU.row].stone == oppositeTurn())
            {
                path.resize(0);
                if(isDead(posU, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(posU);
                }
            }
            if(board[posU.col][posU.row].stone == EMPTY)
                success = true;
        }

        if(success) return true;
        path.resize(0);
        if(isDead(pos, false))
        {
            board[pos.col][pos.row].stone = EMPTY;
        }
        else return true;
    }
    return false;
}

bool Board::isDead(Position pos,const bool opposite)
{
    path.push_back(pos);
    bool dead = true;
    Stone color;
    if(opposite)color = oppositeTurn();
    else color = turn;

    Position posR(pos.col +1, pos.row);
    if(isInRange(posR) && dead)
    {
        if(board[posR.col][posR.row].stone == EMPTY)
            dead = false;
        else if(board[posR.col][posR.row].stone == color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posR));
            if(haventBeen)
                dead = isDead(posR, opposite);
        }
    }

    Position posL(pos.col - 1, pos.row);
    if(isInRange(posL) && dead)
    {
        if(board[posL.col][posL.row].stone == EMPTY)
            dead = false;
        else if(board[posL.col][posL.row].stone == color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posL));
            if(haventBeen)
                dead = isDead(posL, opposite);
        }
    }

    Position posD(pos.col, pos.row + 1);
    if(isInRange(posD) && dead)
    {
        if(board[posD.col][posD.row].stone == EMPTY)
            dead = false;
        else if(board[posD.col][posD.row].stone == color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posD));
            if(haventBeen)
                dead = isDead(posD, opposite);
        }
    }

    Position posU(pos.col, pos.row - 1);
    if(isInRange(posU) && dead)
    {
        if(board[posU.col][posU.row].stone == EMPTY)
            dead = false;
        else if(board[posU.col][posU.row].stone == color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posU));
            if(haventBeen)
                dead = isDead(posU, opposite);
        }
    }
    return dead;
}

void Board::removeDeadGroup(Position pos)
{
    path.push_back(pos);
    Stone color = board[pos.col][pos.row].stone;
    if(color == EMPTY) throw GameException("Cannot remove from empty point.");
    board[pos.col][pos.row].stone = EMPTY;
    if(color == BLACK) bPrisoner++;
    if(color == WHITE) wPrisoner++;

    Position posR(pos.col +1, pos.row);
    if(isInRange(posR))
    {
        bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posR));
        if(haventBeen)
            if(board[posR.col][posR.row].stone == color)
                removeDeadGroup(posR);
    }

    Position posL(pos.col - 1, pos.row);
    if(isInRange(posL))
    {
        bool haventBeen = (path.end()== std::find(path.begin(), path.end(), posL));
        if(haventBeen)
            if(board[posL.col][posL.row].stone == color)
                removeDeadGroup(posL);
    }

    Position posD(pos.col, pos.row + 1);
    if(isInRange(posD))
    {
        bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posD));
        if(haventBeen)
            if(board[posD.col][posD.row].stone == color)
                removeDeadGroup(posD);
    }

    Position posU(pos.col, pos.row - 1);
    if(isInRange(posU))
    {
        bool haventBeen = (path.end() == std::find(path.begin(), path.end(), posU));
        if(haventBeen)
            if(board[posU.col][posU.row].stone == color)
                removeDeadGroup(posU);
    }
}





