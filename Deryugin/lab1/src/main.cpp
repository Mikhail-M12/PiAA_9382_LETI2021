#include <iostream>
#include <stack>

bool IS_SHOW_INTERMEDIATE_RESULTS = false;
//array of names of squares
const char namesOfSquares[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8','9', 'a', 'b', 'c', 'd', 'e', 'i', 'f'};

//class of square
class Square {
public:
    int posX, posY, width;
    bool isMain;
    Square(int x, int y, int w, bool isMain=false) {
        posX = x;
        posY = y;
        width = w;
        this->isMain = isMain;
    }
};

//class of desk
class Desk {
public:
    int minSquares = -1;
    int currentSquare = 1;
    std::stack<Square> minArrayOfSquare;
    std::stack<Square> arrayOfSquares;
    int countOfOperation = 0;
};

Square findMaxSquare(int k , int x, int y, int** matrix) {
    int width = 0;
    int startWith;
    // set max coords
    if (x > y) startWith = x;
    else startWith = y;
    //looking for maximum width of square
    for (int i = startWith; i < k; i++) {
        if (matrix[x + width][y + width] == 0 && matrix[x + width][y] == 0 && matrix[x][y + width] == 0) {
            width++;
        } else {
            break;
        }
    }
    return {x, y, width};
}

void fillMatrix(int** matrix, Square square, Desk* desk) {
    //update desk by new square
    for (int i = square.posX; i < square.posX + square.width; i++) {
        for (int j = square.posY; j < square.posY + square.width; j++) {
            matrix[i][j] = desk->currentSquare;
        }
    }
    desk->currentSquare++;
}

void backtracking(int k , int** matrix, Desk* desk, bool specialCase=false) {
        if (IS_SHOW_INTERMEDIATE_RESULTS) {
            //print desk
            std::cout<<"New desk\n";
            for (int i = 0; i < k; i++) {
                for (int j = 0; j < k; j++) {
                    std::cout<<matrix[i][j]<<" ";
                }
                std::cout<<"\n";
            }
            std::cout<<"\n";
        }
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                // increment count of operations
                desk->countOfOperation++;
                // if ceil isn't empty than skip it
                if (matrix[i][j] != 0) {
                    continue;
                }
                // if current count of squares more than previous count
                if (desk->currentSquare >= desk->minSquares && desk->minSquares != -1) {
                    if (IS_SHOW_INTERMEDIATE_RESULTS) {
                        std::cout<<"Current count of square greater than previous, that's why this desk will be update\n\n";
                    }
                    return;
                }
                //create new max square which can fit at position (i,j)
                Square square = findMaxSquare(k, i, j, matrix);
                // if k multiple of 2, 3 of 5
                if (specialCase) {
                    desk->minArrayOfSquare.push(square);
                }
                else {
                    desk->arrayOfSquares.push(square);
                }
                // fill new square
                fillMatrix(matrix, square, desk);
                if (IS_SHOW_INTERMEDIATE_RESULTS) {
                    std::cout<<"Square with coords ("<<i+1<<";"<<j+1<<") has width "<<square.width<<" was set in desk\n";
                    //print desk
                    for (int i = 0; i < k; i++) {
                        for (int j = 0; j < k; j++) {
                            std::cout<<matrix[i][j]<<" ";
                        }
                        std::cout<<"\n";
                    }
                    std::cout<<"\n";
                }
            }
        }


}

bool decreaseSquare(int k , int **matrix, Desk* desk) {
    if (IS_SHOW_INTERMEDIATE_RESULTS) {
        std::cout<<"Start remove squares\n";
    }
    // change minimum count of squares
    if (desk->currentSquare < desk->minSquares || desk->minSquares == -1) {
        desk->minArrayOfSquare = desk->arrayOfSquares;
        desk->minSquares = desk->currentSquare;
    }

    //remove squares with side equal 1
    while (desk->arrayOfSquares.top().width == 1 ) {
        desk->countOfOperation++;
        Square top = desk->arrayOfSquares.top();
        matrix[top.posX][top.posY] = 0;
        if (IS_SHOW_INTERMEDIATE_RESULTS) {
            std::cout<<"Square with coords ("<<top.posX+1<<";"<<top.posY+1<<") has width "<<top.width<<" was removed\n";
            //print desk
            std::cout<<"Current desk\n";
            for (int i = 0; i < k; i++) {
                for (int j = 0; j < k; j++) {
                    std::cout<<matrix[i][j]<<" ";
                }
                std::cout<<"\n";
            }
            std::cout<<"\n";
        }
        desk->arrayOfSquares.pop();
        desk->currentSquare--;
    }


    Square top = desk->arrayOfSquares.top();
    // end of backtracking
    if (top.isMain) {
        return true;
    }
    // decrease square side by 1
    for (int i = 0; i < top.width; i++) {
        desk->countOfOperation++;
        matrix[top.posX + i][top.posY + top.width - 1] = 0;
        matrix[top.posX + top.width - 1][top.posY + i] = 0;


    }
    if (IS_SHOW_INTERMEDIATE_RESULTS) {
        std::cout<<"Square with coords ("<<top.posX+1<<";"<<top.posY+1<<") has width "<<top.width<<" was decrement width\n";
        //print desk
        std::cout<<"Current desk\n";
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                std::cout<<matrix[i][j]<<" ";
            }
            std::cout<<"\n";
        }
        std::cout<<"\n";
    }
    //decrement width of square
    desk->arrayOfSquares.top().width--;
    return false;

}


void printOptimalDesk(int** matrix, std::stack<Square> squares) {
    // print intermediate results
    if (IS_SHOW_INTERMEDIATE_RESULTS) {
        int numberOfSquare = 1;
        //print desk
        while (!squares.empty()) {
            Square top = squares.top();
            for (int i = top.posX; i < top.posX + top.width; i++) {
                for (int j = top.posY; j < top.posY + top.width; j++) {
                    matrix[i][j] = numberOfSquare;
                }
            }
            numberOfSquare++;
            squares.pop();
        }
    }
    //print answer
    else {
        int count = squares.size();
        std::cout<<count<<std::endl;
        //print coords of squares
        while (!squares.empty()) {
            std::cout<<squares.top().posX + 1<<" "<<squares.top().posY + 1<<" "<<squares.top().width<<std::endl;
            squares.pop();
        }
    }

}

void prepare(int k, int**matrix) {
    Desk desk = Desk();

    // even side
    if (k % 2 == 0) {
        std::cout<<"k % 2 == 0 => min count of square = 4 and side of this squares = k / 2\n";
        Square square(0, 0, k / 2);
        desk.minArrayOfSquare.push(square);
        fillMatrix(matrix, square, &desk);
        backtracking(k, matrix, &desk, true);
    }
    // side multiple of 3
    else if (k % 3 == 0) {
        std::cout<<"k % 3 == 0 => min count of square = 3 and side of the largest square = k / 3 * 2\n";
        Square square(0, 0, k / 3 * 2);
        desk.minArrayOfSquare.push(square);
        fillMatrix(matrix, square, &desk);
        backtracking(k, matrix, &desk, true);
    }
    //side multiple of 5
    else if (k % 5 == 0) {
        std::cout<<"k % 5 == 0 => min count of square = 5 and side of the largest square = k / 5 * 3\n";
        Square square(0, 0, k / 5 * 3);
        desk.minArrayOfSquare.push(square);
        fillMatrix(matrix, square, &desk);
        backtracking(k, matrix, &desk, true);
    }
    //otherwise
    else {
        //create 3 main squares
        Square squareMax(0,0, k / 2 + 1, true);
        Square squareMin1(0, k / 2 + 1, k / 2, true);
        Square squareMin2(k / 2 + 1, 0, k / 2, true);
        desk.arrayOfSquares.push(squareMax);
        fillMatrix(matrix, squareMax, &desk);
        desk.arrayOfSquares.push(squareMin1);
        fillMatrix(matrix, squareMin1, &desk);
        desk.arrayOfSquares.push(squareMin2);
        fillMatrix(matrix, squareMin2, &desk);
        //enumeration of all options
        while (true) {
            backtracking(k, matrix, &desk);
            if(decreaseSquare(k, matrix, &desk)) {
                break;
            }
        }
    }


    //print result
    printOptimalDesk(matrix, desk.minArrayOfSquare);
    // print desk if IS_SHOW_INTERMEDIATE_RESULTS = true
    if (IS_SHOW_INTERMEDIATE_RESULTS) {
        std::cout<<"Optimal desk:\n";
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                std::cout<<namesOfSquares[matrix[i][j]]<<" ";
            }
            std::cout<<"\n";
        }

    }

    std::cout<<"Count of operation with side square equal "<<k<<" - "<<desk.countOfOperation + k * k<<std::endl;
    desk.countOfOperation = 0;
}


int main() {
    int k;
    std::cin>>k;
    //create matrix
    int **matrix = new int*[k];
    for (int i = 0; i < k; i++) {
        matrix[i] = new int[k];
    }

    //fill matrix
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            matrix[i][j] = 0;
        }
    }
    //start backtracking
    prepare(k, matrix);

    //free mem
    for (int i = 0; i < k; i++) {
        delete matrix[i];
    }
    delete[] matrix;

    return 0;
}
