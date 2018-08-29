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
    prevPoints1D = new Point[boardSize*boardSize];
    prevBoard = new Point*[boardSize];
    for(int i = 0; i < boardSize; i++)
        prevBoard[i] = prevPoints1D + i*boardSize;
    prevprevPoints1D = new Point[boardSize*boardSize];
    prevprevBoard = new Point*[boardSize];
    for(int i = 0; i < boardSize; i++)
        prevprevBoard[i] = prevprevPoints1D + i*boardSize;
    newBoard();
    stonesRemoved.reserve(20);
    stonesObserved.reserve(20);
    moves.reserve(200);
}

/// \brief Deallocates memory, that is all.

Board::~Board()
{
    delete[] board;
    delete[] points1D;
    delete[] prevBoard;
    delete[] prevPoints1D;
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
            prevBoard[i][j] = EMPTY;
            prevprevBoard[i][j] = EMPTY;
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
                    if(coord == lastButtonDown) putStone(coord);
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
    // these will be reset just once so that it will hold every observed/removed stone.
    stonesRemoved.resize(0);
    stonesObserved.resize(0);
    //if point is empty
    if(board[coord.col][coord.row] == EMPTY)
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
                if(isDead(coordR))
                {
                    removeDeadGroup(coordR);
                }
            }
        }

        Coordinate coordL(coord.col - 1, coord.row);
        if(isInRange(coordL))
        {
            if(board[coordL.col][coordL.row]== oppositeTurn())
            {
                if(isDead(coordL))
                {
                    removeDeadGroup(coordL);
                }
            }
        }

        Coordinate coordD(coord.col, coord.row + 1);
        if(isInRange(coordD))
        {
            if(board[coordD.col][coordD.row]== oppositeTurn())
            {
                if(isDead(coordD))
                {
                    removeDeadGroup(coordD);
                }
            }
        }

        Coordinate coordU(coord.col, coord.row - 1);
        if(isInRange(coordU))
        {
            if(board[coordU.col][coordU.row]== oppositeTurn())
            {
                if(isDead(coordU))
                {
                    removeDeadGroup(coordU);
                }
            }
        }
        //at this point all surrounding dead stones have been removed
        //so finally we check the stone we have put down
        //this new stone can join previously separate groups, so we have to check recursively for a liberty
        //if not dead, it can be put down, otherwise we remove it from the point
        if(isDead(coord)) //the if argument can only be true if no dead groups have been removed
        {
            board[coord.col][coord.row]= EMPTY;
            return false;
        }
        else
        {
            //this implements ko
            if(compareBoards())
            {
                //stonesRemoved should hold the only removed stone, in case of a ko
                Coordinate removed = stonesRemoved[0];
                board[removed.col][removed.row] = oppositeTurn();
                board[coord.col][coord.row]= EMPTY;
                return false;
            }
            else
            {
                turn = oppositeTurn();
                copyBoards();
                moves.push_back(coord);
                return true;
            }
        }
    }
    return false;
}

/// \brief Examines if group has any liberty.
///
/// \param coord Can be any point of the group examined.
/// \return True if it has no liberties, false otherwise.
///
///


bool Board::isDead(Coordinate coord)
{

    stonesObserved.push_back(coord);
    bool dead = true;
    Point color = board[coord.col][coord.row];
    //We check in every direction if there is a liberty or the group occupies that point as well,
    //if so we invoke the function on the neighbourigóng square (recursion)
    Coordinate coordR(coord.col +1, coord.row);
    if(isInRange(coordR) && dead)
    {
        if(board[coordR.col][coordR.row]== EMPTY)
            dead = false;
        else if(board[coordR.col][coordR.row]== color)
        {
            bool haventBeen = (stonesObserved.end() == std::find(stonesObserved.begin(), stonesObserved.end(), coordR));
            if(haventBeen)
                dead = isDead(coordR);
        }
    }

    Coordinate coordL(coord.col - 1, coord.row);
    if(isInRange(coordL) && dead)
    {
        if(board[coordL.col][coordL.row]== EMPTY)
            dead = false;
        else if(board[coordL.col][coordL.row]== color)
        {
            bool haventBeen = (stonesObserved.end() == std::find(stonesObserved.begin(), stonesObserved.end(), coordL));
            if(haventBeen)
                dead = isDead(coordL);
        }
    }

    Coordinate coordD(coord.col, coord.row + 1);
    if(isInRange(coordD) && dead)
    {
        if(board[coordD.col][coordD.row]== EMPTY)
            dead = false;
        else if(board[coordD.col][coordD.row]== color)
        {
            bool haventBeen = (stonesObserved.end() == std::find(stonesObserved.begin(), stonesObserved.end(), coordD));
            if(haventBeen)
                dead = isDead(coordD);
        }
    }

    Coordinate coordU(coord.col, coord.row - 1);
    if(isInRange(coordU) && dead)
    {
        if(board[coordU.col][coordU.row]== EMPTY)
            dead = false;
        else if(board[coordU.col][coordU.row]== color)
        {
            bool haventBeen = (stonesObserved.end() == std::find(stonesObserved.begin(), stonesObserved.end(), coordU));
            if(haventBeen)
                dead = isDead(coordU);
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
    stonesRemoved.push_back(coord);
    Point color = board[coord.col][coord.row];
    if(color == EMPTY) throw GameException("Cannot remove from empty point.");
    board[coord.col][coord.row]= EMPTY;
    if(color == BLACK) bPrisoner++;
    if(color == WHITE) wPrisoner++;

    Coordinate coordR(coord.col +1, coord.row);
    if(isInRange(coordR))
    {
        //stores whether we have examined that point before, true if we have not
        bool haventBeen = (stonesRemoved.end() == std::find(stonesRemoved.begin(), stonesRemoved.end(), coordR));
        if(haventBeen)
            if(board[coordR.col][coordR.row]== color)
                removeDeadGroup(coordR);
    }

    Coordinate coordL(coord.col - 1, coord.row);
    if(isInRange(coordL))
    {
        bool haventBeen = (stonesRemoved.end()== std::find(stonesRemoved.begin(), stonesRemoved.end(), coordL));
        if(haventBeen)
            if(board[coordL.col][coordL.row]== color)
                removeDeadGroup(coordL);
    }

    Coordinate coordD(coord.col, coord.row + 1);
    if(isInRange(coordD))
    {
        bool haventBeen = (stonesRemoved.end() == std::find(stonesRemoved.begin(), stonesRemoved.end(), coordD));
        if(haventBeen)
            if(board[coordD.col][coordD.row]== color)
                removeDeadGroup(coordD);
    }

    Coordinate coordU(coord.col, coord.row - 1);
    if(isInRange(coordU))
    {
        bool haventBeen = (stonesRemoved.end() == std::find(stonesRemoved.begin(), stonesRemoved.end(), coordU));
        if(haventBeen)
            if(board[coordU.col][coordU.row]== color)
                removeDeadGroup(coordU);
    }
}

bool Board::compareBoards()
{
    bool same = true;
    int i, j;
    for(i = 0, j = 0; i < boardSize; i++)
        for(j = 0; j < boardSize; j++)
    {
        if(board[i][j] != prevprevBoard[i][j]) same = false;
    }
    return same;
}

void Board::copyBoards()
{
    int i, j;
    for(i = 0; i < boardSize; i++)
        for(j = 0; j < boardSize; j++)
    {
        prevprevBoard[i][j] = prevBoard[i][j];
        prevBoard[i][j] = board[i][j];
    }
}




