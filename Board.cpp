#include "Board.h"

/// \brief Dynamically allocates for the board, by default is 19x19
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

Board::~Board()///< Deallocates memory.
{
    delete[] board;
    delete[] points1D;
}

/// \brief Loads a board position from binary file. \todo It could handle .sgf as well.
/// \return true if succeeded
///

bool Board::loadBoard()
{
    return true;
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

/// \brief Handles mouse clicks. Currently it does not distinguish between mousebuttons.
/// \param event Next event from queue
/// \param stoneTexSize Needs to have this to translate between pixel coordinate and point coordinate.
///

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
                        // registering that mouse has been clicked, also over which point
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
                    Coordinate coord(x / stoneTexSize, y / stoneTexSize);
                    //if mouse is still in the square
                    if(coord == lastButtonDown)
                        {
                            if(putStone(coord)) turn = oppositeTurn();
                        }
                }
                mouseButtonDown = false;
            }
            break;
    }
}

/// \brief Checks if move is legal, then puts a stone of appropriate color. Removes dead stones if necessary.
/// \param coord Where to put stone
/// \return true, if stone has been put
///

bool Board::putStone(Coordinate coord)
{
    bool success = false;
    if(board[coord.col][coord.row]== EMPTY)
    {
        board[coord.col][coord.row]= turn;

        Coordinate coordR(coord.col +1, coord.row);
        if(isInRange(coordR))
        {
            if(board[coordR.col][coordR.row]== oppositeTurn())
            {
                path.resize(0);
                if(isDead(coordR, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(coordR);
                }
            }
            if(board[coordR.col][coordR.row]== EMPTY)
                success = true;
        }

        Coordinate coordL(coord.col - 1, coord.row);
        if(isInRange(coordL))
        {
            if(board[coordL.col][coordL.row]== oppositeTurn())
            {
                path.resize(0);
                if(isDead(coordL, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(coordL);
                }
            }
            if(board[coordL.col][coordL.row]== EMPTY)
                success = true;
        }

        Coordinate coordD(coord.col, coord.row + 1);
        if(isInRange(coordD))
        {
            if(board[coordD.col][coordD.row]== oppositeTurn())
            {
                path.resize(0);
                if(isDead(coordD, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(coordD);
                }
            }
            if(board[coordD.col][coordD.row]== EMPTY)
                success = true;
        }

        Coordinate coordU(coord.col, coord.row - 1);
        if(isInRange(coordU))
        {
            if(board[coordU.col][coordU.row]== oppositeTurn())
            {
                path.resize(0);
                if(isDead(coordU, true))
                {
                    success = true;
                    path.resize(0);
                    removeDeadGroup(coordU);
                }
            }
            if(board[coordU.col][coordU.row]== EMPTY)
                success = true;
        }

        if(success) return true;
        path.resize(0);
        if(isDead(coord, false))
        {
            board[coord.col][coord.row]= EMPTY;
        }
        else return true;
    }
    return false;
}

bool Board::isDead(Coordinate coord,const bool opposite)
{
    path.push_back(coord);
    bool dead = true;
    Point color;
    if(opposite)color = oppositeTurn();
    else color = turn;

    Coordinate coordR(coord.col +1, coord.row);
    if(isInRange(coordR) && dead)
    {
        if(board[coordR.col][coordR.row]== EMPTY)
            dead = false;
        else if(board[coordR.col][coordR.row]== color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), coordR));
            if(haventBeen)
                dead = isDead(coordR, opposite);
        }
    }

    Coordinate coordL(coord.col - 1, coord.row);
    if(isInRange(coordL) && dead)
    {
        if(board[coordL.col][coordL.row]== EMPTY)
            dead = false;
        else if(board[coordL.col][coordL.row]== color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), coordL));
            if(haventBeen)
                dead = isDead(coordL, opposite);
        }
    }

    Coordinate coordD(coord.col, coord.row + 1);
    if(isInRange(coordD) && dead)
    {
        if(board[coordD.col][coordD.row]== EMPTY)
            dead = false;
        else if(board[coordD.col][coordD.row]== color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), coordD));
            if(haventBeen)
                dead = isDead(coordD, opposite);
        }
    }

    Coordinate coordU(coord.col, coord.row - 1);
    if(isInRange(coordU) && dead)
    {
        if(board[coordU.col][coordU.row]== EMPTY)
            dead = false;
        else if(board[coordU.col][coordU.row]== color)
        {
            bool haventBeen = (path.end() == std::find(path.begin(), path.end(), coordU));
            if(haventBeen)
                dead = isDead(coordU, opposite);
        }
    }
    return dead;
}

void Board::removeDeadGroup(Coordinate coord)
{
    path.push_back(coord);
    Point color = board[coord.col][coord.row];
    if(color == EMPTY) throw GameException("Cannot remove from empty point.");
    board[coord.col][coord.row]= EMPTY;
    if(color == BLACK) bPrisoner++;
    if(color == WHITE) wPrisoner++;

    Coordinate coordR(coord.col +1, coord.row);
    if(isInRange(coordR))
    {
        bool haventBeen = (path.end() == std::find(path.begin(), path.end(), coordR));
        if(haventBeen)
            if(board[coordR.col][coordR.row]== color)
                removeDeadGroup(coordR);
    }

    Coordinate coordL(coord.col - 1, coord.row);
    if(isInRange(coordL))
    {
        bool haventBeen = (path.end()== std::find(path.begin(), path.end(), coordL));
        if(haventBeen)
            if(board[coordL.col][coordL.row]== color)
                removeDeadGroup(coordL);
    }

    Coordinate coordD(coord.col, coord.row + 1);
    if(isInRange(coordD))
    {
        bool haventBeen = (path.end() == std::find(path.begin(), path.end(), coordD));
        if(haventBeen)
            if(board[coordD.col][coordD.row]== color)
                removeDeadGroup(coordD);
    }

    Coordinate coordU(coord.col, coord.row - 1);
    if(isInRange(coordU))
    {
        bool haventBeen = (path.end() == std::find(path.begin(), path.end(), coordU));
        if(haventBeen)
            if(board[coordU.col][coordU.row]== color)
                removeDeadGroup(coordU);
    }
}





