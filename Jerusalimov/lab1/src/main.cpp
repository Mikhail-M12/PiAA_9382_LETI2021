#include <iostream>
#include <vector>
#include <array>


int choise = 1;




struct Cell {
    int size;
    int x, y;
};

class Square {
public:

    Square(int size,int comp) {
        this->size = size;
        this->comp = comp;
        count = 0;
        ++countOp;
    }

    ~Square() {
    }

    void setSquare(int x, int y, int size) {
        ++(this->count);
        for (int i = y; i < y + size; ++i) {
            for (int j = x; j < x + size; ++j) {
                this->map[i][j] = size;
            }
        }
        Cell newSquare;
        newSquare.x = x;
        newSquare.y = y;
        newSquare.size = size;
        squareStack.push_back(newSquare);
        countOp += 4 + (size - 1) * (size - 1);
    }

    int canSetSquareinRange(Cell range) {
        --range.size;
        int sizeSmallSquare = 1;
        while (sizeSmallSquare < range.size && (range.x + sizeSmallSquare) < size &&
               (range.y + sizeSmallSquare) < size &&
               !(this->map[range.y][range.x + sizeSmallSquare])) {
            ++sizeSmallSquare;
            countOp += 7;
        }

        return sizeSmallSquare;
    }

    Cell removeSquare(int x, int y, int size) {
        --(this->count);
        for (int i = y; i < y + size; ++i) {
            for (int j = x; j < x + size; ++j) {
                this->map[i][j] = 0;
            }
        }
        Cell temp = squareStack.back();
        squareStack.pop_back();
        countOp += 3 + (size - 1) * (size - 1);
        return temp;
    }

    Cell whereEmpty(int x, int y) {
        Cell arr;
        while (this->map[y][x]) {
            if (x == this->size - 1) {
                if (y == this->size - 1) {
                    return arr = {-1, -1, -1};
                }
                x = this->size / 2;
                ++y;
                countOp += 4;
            } else {
                ++x;
            }

        }
        arr = {0, x, y};
        countOp += 3;
        return arr;
    }

    Square *copy() {
        Square *newSq = new Square(this->size,this->comp);
        for (int i = 0; i < this->size; ++i) {
            for (int j = 0; j < this->size; ++j) {
                newSq->map[i][j] = this->map[i][j];
            }
        }
        newSq->count = this->count;
        newSq->squareStack = this->squareStack;
        countOp += 3 + ((size - 1) * (size - 1));
        return newSq;
    }

    std::array<std::array<int,30>,30> map;
    std::vector<Cell> squareStack; //стек наших кубиков.
    int size;                      //размер полатна
    int count;                     // счетчик квадратов
    int comp;
    int countOp = 0;
};

//template<typename Type>
void printMessage(const char* message){
    if (choise == 1) {
        std::cout<<message;
    }
}

void printSquare(Square *some, bool flagIsMap) {
    //bool is_same_v = std::is_same < Type, Square > :: value ;
    if (choise == 1) {
      if ( flagIsMap) {

            for (int i = 0; i < some->size; ++i) {
                for (int j = 0; j < some->size; ++j) {
                    std::cout << some->map[i][j] << " ";
                }
                std::cout << "\n";
            }

        } else {
            for (int i = 0; i < some->squareStack.size(); ++i) {
                std::cout << some->squareStack[i].x * some->comp + 1 << " "
                          << some->squareStack[i].y * some->comp + 1 << " "
                          << some->squareStack[i].size * some->comp << "\n";
            }
        }
    }

}


class Solve {
public:
    Solve(int size) {
        comp = 1;
        this->size = compression(size, comp);
        curMap = new Square(size,comp);
        easyMap = new Square(size,comp);
    }

    ~Solve() {
        delete curMap;
        delete easyMap;
    }

    int compression(int size, int &comp) {
        int compSize = size;
        for (int i = size - 1; i > 1; --i) {
            if (!(size % i)) {
                comp = i;
                compSize = size / i;
                countop += 2;
                break;
            }
        }
        countop += 1 + (size - 2);
        return compSize;
    }

    void bestSquare() {
        easyMap->setSquare(0, 0, size - 1);
        for (int y = 0; y < size; ++y) {
            easyMap->setSquare(size - 1, y, 1);
        }
        for (int x = 0; x < size - 1; ++x) {
            easyMap->setSquare(x, size - 1, 1);

        }
    }

    void currentMap() {
        curMap->countOp += 3;
        curMap->setSquare(0, 0, size / 2 + 1);
        curMap->setSquare(size / 2 + 1, 0, size / 2);
        curMap->setSquare(0, size / 2 + 1, size / 2);
    }


    void solve() {
        currentMap();
        printMessage("Add 3 square with this parameters:\nx y size|\n");
        printSquare(easyMap,0);
        printMessage("This is the most optimal way.\n\nVisualization:\n");
        printSquare(curMap,1);
        int x = size / 2 + 1;
        int y = size / 2;
        curMap->countOp += 2;

        Cell tempS;

        printMessage("\nTry find free space...\n");
        tempS = curMap->whereEmpty(x, y);
        tempS.size = size - 1;
        int sizeOfCanSet;
        curMap->countOp += 3;
        printMessage("Found an empty space on: ");

        if (choise) {
            std::cout << "{ " << tempS.x + 1 << ", " << tempS.y + 1 << " }\n";
        }

        while (true) {
            curMap->countOp += 2;
            printMessage("The big cycle has begun...\n");
            if (curMap->count == 2) {
                printMessage("Removed the 3-d square. \nIt makes no sense to continue "
                      "further, all the best options have already been found.\n");
                break;
            }

            bool full = false;

            while (!full) {

                //¬ыбираем самый большой квадрат который можно вставить в эту область.
                printMessage("\n\tThe cycle of filling has begun...\n");
                sizeOfCanSet = curMap->canSetSquareinRange(tempS);
                if (choise) {
                    printMessage("\tWe are looking for the largest square that can be put on: ");
                    std::cout << "{ " << tempS.x + 1 << ", " << tempS.y + 1
                              << " } in range " << tempS.size << ", ";
                    printMessage("this is it: ->");

                    std::cout << " " << sizeOfCanSet << "\n";
                }

                // insert square

                curMap->setSquare(tempS.x, tempS.y, sizeOfCanSet);
                printMessage("\tSet a square to these coordinates:\n\tx y size\n");
                if (choise) {
                    std::cout << "\t" << curMap->squareStack.back().x * comp + 1 << " "
                              << curMap->squareStack.back().y * comp + 1 << " "
                              << curMap->squareStack.back().size * comp
                              << "\n\n\tVisualization:\n";
                }
                printSquare(curMap,1);

                //≈сли кол-во квадратов в текущем больше или равно лучшего варианта. Ќе
                //продолжать. —мысла нет.

                if (curMap->count >= easyMap->count) {
                    printMessage("\tThe number of squares in the new case is more than in the "
                          "best.\nExit the fill cycle\n");
                    break;
                }


                printMessage("\n\tTry find free space...\n");
                tempS = curMap->whereEmpty(x, y);

                if (tempS.size == -1) {
                    full = true;
                    ++curMap->countOp;
                    printMessage("\tNot found an empty space.\nExit the fill cycle\n");

                } else {

                    printMessage("\tFound an empty space on: ");
                    if (choise) {
                        std::cout << "{ " << tempS.x + 1 << ", " << tempS.y + 1 << " }\n";
                    }
                }
                tempS.size = size - (size + 1) / 2;
                curMap->countOp += 6;
            }

            if (full && (curMap->count < easyMap->count)) {

                if (choise) {
                    std::cout << "Best map count  " << easyMap->count << " >  "
                              << curMap->count << " new map count\n";
                }
                ++curMap->countOp;
                easyMap = curMap->copy();

                printMessage("Copied the new version to the best one. Since there are fewer "
                      "squares\n\nVisualization new Best Map:\n");
                printSquare(curMap,1);
            }

            printMessage("\n______________________________Remove to try new "
                  "Map____________________________\nCycle removed start!\n");
            do {
                curMap->countOp += 6;
                printMessage("\nRemove square at this pos:\nx y size\n");
                if (choise) {
                    std::cout << curMap->squareStack.back().x * comp + 1 << " "
                              << curMap->squareStack.back().y * comp + 1 << " "
                              << curMap->squareStack.back().size * comp
                              << "\n\nVisualization:\n";
                }
                tempS = curMap->removeSquare(curMap->squareStack.back().x,
                                             curMap->squareStack.back().y,
                                             curMap->squareStack.back().size);
                printSquare(curMap,1);
                printMessage("\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
                      "||||||||||||||||||||\n");
            } while (curMap->count > 2 && tempS.size == 1);
            curMap->countOp += 5;
            printMessage("\nCycle removed end.\n______________________________________End "
                  "Removing__________________________________\n");
        }
    }

    void solveEven() {
        curMap->countOp += 4;
        easyMap->setSquare(0, 0, 1);
        easyMap->setSquare(0, 1, 1);
        easyMap->setSquare(1, 0, 1);
        easyMap->setSquare(1, 1, 1);
    }

    void fillSquare() {

        printMessage("Getting started filling the "
              "square!\n___________________________________\n");
        if (!(this->size % 2)) {
            curMap->countOp += 1;
            printMessage("The side is even!\n When the side is even, the square is divided "
                  "into 4 equal squares. This will be the answer.\n");
            solveEven();

        } else {
            bestSquare();
            solve();
            curMap->countOp += 2;
            easyMap->countOp = curMap->countOp + countop;
        }

        return;
    }

    Square *easyMap; // лучший вариант
    Square *curMap;  // текущий вариант
    int comp; //сжатие дл€ четной стороны квадрата. » дл€ нечетной не простой
    //стороны
    int size;
    int countop = 0;

private:
};




int main() {
    int SizeOfSide;

//    std::cout << "Logs?\n0 - No || 1 - Yes\n";
//    std::cin >> choise;
//
//    while (choise < 0 || choise > 1) {
//        std::cout << "Wrong Enter, try again.\n";
//        std::cin.clear();
//        std::cin >> choise;
//    }

    std::cout << "Enter the length of the square: \n";
    std::cin >> SizeOfSide;
    Solve ml(SizeOfSide);

    std::cout << "Created a square with a side " << SizeOfSide << ".\n";
    ml.fillSquare();

    std::cout << "\nResult!" << "\n____________________________________________________\n";
    printSquare(ml.easyMap,0);
    std::cout << "\n count operation: " << ml.easyMap->countOp << "\n";
    std::cout << "\nBest map is:\n";

    printSquare(ml.easyMap,1);
    return 0;
}