#include "./SquarePavingMap/SquarePavingMap.h"

void printStack(const std::vector<Square>& st)
{
    for(auto &square : st)
    {
        std::cout << "{" << square.x << ", " << square.y << ", " << square.size << "} ";
    }
    std::cout << '\n';
}

unsigned int smallestDerivative(unsigned int n)
{
    unsigned int i = 3;
    if (n % 2 == 0) return 2;
    else {
        while(n%i && i*i <= n) {
            i += 2;
        }
        return ((n%i) ? n : i);
    }
}

class SquarePaver
{
private:
    std::vector<Square> bestPaving;
    std::vector<Square> currentPaving;

    unsigned int squarePavingMapSize;
    SquarePavingMap squarePavingMap;

    void addSquare(Square square)
    {
        currentPaving.push_back(square);
        squarePavingMap.add(square);
    }

    void removeLastSquare()
    {
        Square lastSquare = currentPaving.back();
        currentPaving.pop_back();
        squarePavingMap.remove(lastSquare);        
    }

    void removeLastSmallSquares()
    {
        Square lastSquare = currentPaving.back();
        while(lastSquare.size == 1 && currentPaving.size() > 1)
        {
            removeLastSquare();
            lastSquare = currentPaving.back();
        }
    }

    void updatePaving()
    {
        bestPaving = currentPaving;
    }

    void viewBestPaving()
    {
        std::cout << "\nBestPavingMap -- \n";
        printStack(bestPaving);
    }

    void viewCurrentPaving()
    {
        std::cout << "\nCurrentPavingMap -- \n";
        printStack(currentPaving);
    }    

    bool possibleToFindBetterPaving()
    {
        return ((bestPaving.size() == 0) || currentPaving.size() < bestPaving.size());
    }

public:
    SquarePaver(unsigned int size)
    {
        squarePavingMapSize= size;
        squarePavingMap = SquarePavingMap(size);

        Square square = {0, 0, (size + 1) / 2};
        addSquare(square);

        square = {0, (size + 1) / 2, size - (size + 1) / 2};
        addSquare(square);

        square = {(size + 1) / 2, 0, size - (size + 1) / 2};
        addSquare(square);
    }

    void view()
    {
        viewBestPaving();
        viewCurrentPaving();
        std::cout << "\nsquarePavingMap -- \n";
        squarePavingMap.view();
        std::cout << '\n';
    }

    void findBestPaving()
    {
        while (currentPaving.size() >= 3) // Because if we have changed initialisited squares we are out of options.
        {
            Point firstFree = squarePavingMap.giveFirstFree();
            Square lastSquare;
            while ((firstFree.x != -1 && possibleToFindBetterPaving()))
            {
                std::cout << "found free place for square -- (" << firstFree.x << ", " << firstFree.y << ")\n";
                unsigned int currentMaxSize = squarePavingMap.giveMaxSquareSize(firstFree);
                lastSquare = Square{(unsigned int) firstFree.x, (unsigned int) firstFree.y, currentMaxSize};

                std::cout << "Placing square...\n";
                std::cout << "Now the system state looks like this:\n\n";
                view();
                std::cout << "\nEnd of state\n\n";
                addSquare(lastSquare);
                firstFree = squarePavingMap.giveFirstFree();
            }
            std::cout << "It's impossible to make progress now\n";
                

            if (bestPaving.size() == 0 || bestPaving.size() > currentPaving.size())
            {
                std::cout << "Better paving has been found\n";
                updatePaving();
            }
            
            // What to do when the map is filled? Delete all squares == 1 and one square after that

            std::cout << "Deleting small squares... and one behind them\n";

            removeLastSmallSquares();

            if (currentPaving.size() <= 3) // Small optimtsation
                break;
            
            lastSquare = currentPaving.back();
            removeLastSquare();
            lastSquare.size--;
            addSquare(lastSquare);
        }
    }

    void viewFancyWay(unsigned int modifier)
    {
        std::cout << bestPaving.size() << '\n';
        for(auto &square : bestPaving)
        {
            std::cout << (square.x*modifier)+1 << " " << (square.y*modifier)+1 << " " << (square.size*modifier) << '\n';
        }
    }
    
};

int main()
{
    unsigned int size;
    std::cin >> size;
    unsigned int n = smallestDerivative(size);
    unsigned int modifier = size/n;

    SquarePaver squarePaver(n);
    squarePaver.findBestPaving();
    std::cout << "The answer was found\n";
    squarePaver.viewFancyWay(modifier);

    return 0;
}