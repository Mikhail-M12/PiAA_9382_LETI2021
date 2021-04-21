#include "SquarePavingMap.h"


SquarePavingMap::SquarePavingMap(const unsigned int inputSize)
{
    if (inputSize <= MAX_MAP_SIZE)
        size = inputSize;
    
    for(int i=0; i<MAX_MAP_SIZE; i++)
    {
        for(int j=0; j<MAX_MAP_SIZE; j++)
            map[i][j] = false;
    }
}

bool SquarePavingMap::isInBound(int coord)
{
    return (0 <= coord && coord < size);
}

void SquarePavingMap::add(Square square)
{
    for (int i = square.x; (i<square.x + square.size) && isInBound(i); i++)
    {
        for (int j = square.y; (j<square.y + square.size) && isInBound(j); j++)
            map[i][j] = true;
    }
}

void SquarePavingMap::remove(Square square)
{
    for (int i = square.x; (i<square.x + square.size) && isInBound(i); i++)
    {
        for (int j = square.y; (j<square.y + square.size) && isInBound(j); j++)
            map[i][j] = false;
    }
}

void SquarePavingMap::view()
{
    for (int i=0; i<size; i++)
    {
        for (int j=0; j<size; j++)
            std::cout << map[i][j] << ' ';
        std::cout << "\n";
    }
}

void SquarePavingMap::viewAll()
{
    for (int i=0; i<MAX_MAP_SIZE; i++)
    {
        for (int j=0; j<MAX_MAP_SIZE; j++)
            std::cout << map[i][j] << ' ';
        std::cout << "\n";
    }
}


Point SquarePavingMap::giveFirstFree()
{
    for (int i=0; i<size; i++)
    {
        for (int j=0; j<size; j++)
            if (map[i][j] == false) return Point{i, j};
    }
    return Point{-1, -1};
}

unsigned int SquarePavingMap::giveMaxSquareSize(Point point)
{
    int currentSize = 0, x = point.x, y = point.y;
    bool flag = true;
    do
    {
        currentSize++;
        for (int i=0; i<currentSize; i++)
        {
            if (isInBound(x) && isInBound(y-i))
            {
                if (map[x][y-i])
                {
                    flag = false;
                    break;
                }
            }
            else
            {
                flag = false;
                break;
            }
        
            if (isInBound(x-i) && isInBound(y))
            {
                if (map[x-i][y])
                {
                    flag = false;
                    break;
                }
            }
            else
            {
                flag = false;
                break;
            }
        }
        x++;
        y++;
    } while (flag);
    currentSize--;
    return (currentSize);
}