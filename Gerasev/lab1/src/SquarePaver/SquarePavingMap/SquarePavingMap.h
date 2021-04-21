#include <iostream>
#include <vector>
#include <array>

#define MAX_MAP_SIZE 40

struct Square
{
    unsigned int x, y, size;
};

struct Point
{
    int x, y;
};

class SquarePavingMap
{
private:
    unsigned int size = 0;
    std::array< std::array<bool, MAX_MAP_SIZE>, MAX_MAP_SIZE> map;

    void viewAll();

public:
    SquarePavingMap(const unsigned int inputSize = 0);
    bool isInBound(int coord);
    void add(Square square);
    void remove(Square square);
    unsigned int giveMaxSquareSize(Point point);
    Point giveFirstFree();
    void view();
};