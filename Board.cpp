#include "Board.h"

Board::Board():
    boardSize(19),
    point1D(NULL),
    point(NULL),
    turn(BLACK),
    mouse(UP)
{
    point1D = new Point[boardSize*boardSize];
    point = new Point*[boardSize];
    for(int i = 0; i < boardSize; i++)
        point[i] = point1D + i*boardSize;
    newBoard();
}

Board::~Board()
{
    delete[] point;
    delete[] point1D;

}

void Board::newBoard()
{
    int j = 0;
    for(int i = 0; i < boardSize; i++)
    {
        for(j = 0; j < boardSize; j++)
        {
            point[i][j].x = i*25;
            point[i][j].y = j*25;
            point[i][j].isTransparent = false;
            point[i][j].stone = EMPTY;
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
                if(isInRange(col, row) && point[col][row].stone == EMPTY)
                    point[col][row].isTransparent = true;
            }
    }
}


