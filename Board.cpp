#include "Board.h"

/// \brief Dynamically allocates memory for the board, by default board is 19x19
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

/// \brief Handles mouse clicks. \todo Currently it does not distinguish between mouse buttons.
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

/// \brief Checks if move is legal, then puts a stone of appropriate color.
/// Removes dead stones if necessary by calling \code removeDeadGroup() \endcode .
/// \param coord Where to place stone
/// \return true, if stone has been put
///

bool Board::putStone(Coordinate coord)
{
    bool success = false;
    //if point is empty
    if(board[coord.col][coord.row]== EMPTY)
    {
        //we put down the stone, then check the resulting situation
        //until we decide if it is legal to do, it remains a theoretical move
        board[coord.col][coord.row]= turn;
        //we look all four directions: Right Left Down Up
        Coordinate coordR(coord.col +1, coord.row);
        if(isInRange(coordR))
        {
            //we check for opposite colors around
            if(board[coordR.col][coordR.row]== oppositeTurn())
            {
                // isDead() and removeDeadGroup() needs path to be emptied.
                // Path variable will hold the points along which we 'walk around' in the group
                // success means we either succeeded to remove a group, or the point had been empty anyway.
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
        //lastly we check the stone we have put down
        //this new stone can join previously separate groups, so we have to check recursively for a liberty
        //if not dead, it can be put down, otherwise we remove it from the point
        path.resize(0);
        if(isDead(coord, false))
        {
            board[coord.col][coord.row]= EMPTY;
        }
        else return true;
    }
    return false;
}

/// \brief Examines if group has any liberty.
///
/// \param coord Can be any point of the group examined.
/// \param opposite True if we examine a group whose color is opposite of the stone put
/// \return True if it has no liberties, false otherwise.
///
///


bool Board::isDead(Coordinate coord,const bool opposite)
{
    path.push_back(coord);
    bool dead = true;
    // It needs to know what color we are dealing with. if opposite of the color of the player playing the stone,
    // then stones will be removed, If same color, It is an illegal move.
    Point color;
    if(opposite)color = oppositeTurn();
    else color = turn;
    //In every direction we check if there is a liberty or the group occupiesthat point as well,
    //if so we invoke the function on the neighbourigóng square (recursion)
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

/// \brief Removes groups. It needs only one stone's coordinates to remove the group. Does not examine if group has liberties.
/// Follows the same recursive logic as functions before.
///
/// \param coord This is the coordinate of any member of the group.
///


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
        //stores wheter we have examined that point before, true if we have not
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





