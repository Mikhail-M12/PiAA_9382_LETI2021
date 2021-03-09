#include <vector>
#include <algorithm>
#include <iostream>

int count = 0;

class Square{

private:
    int size=0;
    int x_coordinate=0;//top down
    int y_coordinate=0;//from left to right
public:
    Square()=default;
    Square(int s, int x, int y): size(s), x_coordinate(x), y_coordinate(y){}
    ~Square() = default;
    int GetXCoordinate();
    int GetYCoordinate();
    int GetSize();
};

class Handler{

public:  
    bool CheckAllMatrix(int** matrix, int matrix_size, int& x_coordinate, int& y_coordinate, std::vector<Square>& vector, int vector_size);
    void PrintMatrix(int** matrix, int marix_size);
    void PrintVector(std::vector<Square> vector);
    void PopBackVector(int** matrix, int matrix_size, std::vector<Square>& vector, int& vector_size, int& flag);
    int FindNewSizeRightDown(int** matrix, int matrix_size, int x_coordinate, int y_coordinate);
    void SetSquare(int** matrix, int matrix_size, int x_coordinate, int y_coordinate, int square_size, int color);

};

int Square::GetXCoordinate(){
    return this->x_coordinate;
}

int Square::GetYCoordinate(){
    return this->y_coordinate;
}

int Square::GetSize(){
    return this->size;
}

int Handler::FindNewSizeRightDown(int** matrix, int matrix_size, int x_coordinate, int y_coordinate){
    int right_size = 1, down_size = 1;
    int i = x_coordinate, j = y_coordinate;

    int max_size = matrix_size-1;

    count+=5;
    
    //find a square with maximum size in this coordinates
    while (right_size <= max_size && j+right_size<matrix_size && matrix[i][j+right_size]!=1 ){
        right_size++;
        count+=4;//increase a right_size and check values in condition
    } 

    while (down_size <= max_size && i+down_size<matrix_size && matrix[i+down_size][j]!=1 ){
        down_size++;
        count+=4;//increase a right_size and check value in matrix
    }
    int size=std::min(right_size, down_size);
    std::cout<<"Find new square at coordinates: ("<<x_coordinate<<", "<<y_coordinate<<")... Find a square size: "<<size<<"\n";
    return size;
}

void Handler::SetSquare(int** matrix, int matrix_size, int x_coordinate, int y_coordinate, int square_size, int color){
    //set square of ones with given size and coordinates in matrix
    for (int i = x_coordinate; i < x_coordinate + square_size; i++){
        for (int j = y_coordinate; j < y_coordinate + square_size;j++){
            matrix[i][j]=color;
        }
    }
    count+=square_size*square_size;
    
}

bool Handler::CheckAllMatrix(int** matrix, int matrix_size, int& x_coordinate, int& y_coordinate, std::vector<Square>& vector, int vector_size){
    std::cout<<"Cheking free space in matrix...\n";
    for (int i=0;i<matrix_size;i++){
        for (int j=0;j<matrix_size;j++){
            count+=1;
            if (matrix[i][j]==0){//if find a free cell
                x_coordinate = i;
                y_coordinate = j;
                count+=2;
                std::cout<<"Find free space at coordinates: ("<<x_coordinate<<", "<<y_coordinate<<")\n";
                return true;
            }
        }
    }
    std::cout<<"Free space didn't find\n";
    return false;
}

void Handler::PopBackVector(int** matrix, int matrix_size, std::vector<Square>& vector, int& vector_size, int& flag){
   
   while (true){
       if (vector_size<=3){
           flag=1;
           count+=2;
           std::cout<<"All optimal candidate for filling square was find, stoped cheking\n";
           break;
        } else{
            Square square = vector[vector_size-1];
            count+=1;
            if (vector[vector_size-1].GetSize()>=2){//delete the last value in vector, push value with decreasing size
                
                vector.pop_back();
                vector_size-=1;
                this->SetSquare(matrix, matrix_size, square.GetXCoordinate(), square.GetYCoordinate(), square.GetSize(), 0);
                this->SetSquare(matrix, matrix_size, square.GetXCoordinate(), square.GetYCoordinate(), square.GetSize()-1, 1);
                vector.push_back(Square(square.GetSize()-1, square.GetXCoordinate(), square.GetYCoordinate()));
                vector_size+=1;
                count+=7;
                break;
            }
            else{//deletethe last value in vector
                vector.pop_back();
                vector_size-=1;
                this->SetSquare(matrix, matrix_size, square.GetXCoordinate(), square.GetYCoordinate(), square.GetSize(), 0);
                count+=4;
                continue;
            }
        }
    }
}

void Handler::PrintMatrix(int** matrix, int matrix_size){
    for (int i=0;i<matrix_size;i++){
        for (int j=0;j<matrix_size;j++){
            std::cout<<matrix[i][j]<<' ';
        }
        std::cout<<'\n';
    }
    count+=matrix_size*matrix_size;
}

void Handler::PrintVector(std::vector<Square> vector){
    
    for (int i=0;i<vector.size();i++){
        std::cout<<vector[i].GetSize()<<' '<<vector[i].GetXCoordinate()<<' '<<vector[i].GetYCoordinate()<<", ";
    }
    count+=3*vector.size();
    std::cout<<"\n";
}

int main(){
    
    std::vector <Square> finish;//candidate of result suquence
    std::vector <Square> current;//current path
    Handler handler;

    int n;
    std::cin>>n;

    int result_size = n*n;//max size, if squares has sizes 1

    count+=2;//for following and previous operations

    if (n==2){
        std::cout<<'4'<<"\n";
        std::cout<<"1 1 1\n1 2 1\n2 1 1\n2 2 1\n";
        std::cout<<"\nAll number of operations for square size "<< n <<" = "<<count<<"\n";

        return 0;
    }
    
    count+=1;
    if (n%2==0){//especial case
        result_size = 4;
        finish.push_back(Square(n/2, 0, 0));
        finish.push_back(Square(n/2, 0, n/2));
        finish.push_back(Square(n/2, n/2, 0));
        finish.push_back(Square(n/2, n/2, n/2));
        count+=5;
    }
    else{

        int** matrix = new int*[n];

        for (int i=0; i<n; i++){
            matrix[i]= new int[n];
        }

        count+=2*n+1;

        for (int i=0;i<n;i++){
            for (int j=0;j<n;j++){
                matrix[i][j]=0;
            }
        }
        count+=n*n;


        if (n%3==0){
            current.push_back(Square(n*2/3, 0, 0));
            current.push_back(Square(n/3, 0, n*2/3));
            current.push_back(Square(n/3, n*2/3,0));

            handler.SetSquare(matrix, n, 0, 0, n*2/3, 1);
            handler.SetSquare(matrix, n, 0, n*2/3, n/3, 1);
            handler.SetSquare(matrix, n, n*2/3, 0, n/3, 1);

            count+=4;
        }
        else{
            current.push_back(Square(n/2 + 1, 0, 0));
            current.push_back(Square(n/2, 0, n/2+1));
            current.push_back(Square(n/2, n/2+1,0));

            handler.SetSquare(matrix, n, 0, 0, n/2+1, 1);
            handler.SetSquare(matrix, n, 0, n/2+1, n/2, 1);
            handler.SetSquare(matrix, n, n/2+1, 0, n/2, 1);
            count+=4;
        }

        int current_x = 0;
        int current_y = 0;
        int current_max_size = 3;
        int new_square_size = 0;
        int flag=0;

        count+=5;


        while (!current.empty() && flag!=1){//while not checked all optimal decisions
            count+=2;

            std::cout<<"\nCurrent path: ";
            handler.PrintVector(current);
            
            std::cout<<"\nCurrent matrix:\n";
            handler.PrintMatrix(matrix, n);  

            count+=3;
            if (n>11 && current_max_size>=n){
                handler.PopBackVector(matrix, n, current, current_max_size, flag);
                continue;
            }

            count+=1;
            if (current_max_size>=result_size){
                std::cout<<"Current path size is longer then finish path size, stop cheking further\n";
                handler.PopBackVector(matrix, n, current, current_max_size, flag);
                continue;
            }
            
            //set coordinates in free cell or delete values in vector if free cell din't find
            if (handler.CheckAllMatrix(matrix, n, current_x, current_y, current, current_max_size)){
                int new_size = handler.FindNewSizeRightDown(matrix, n, current_x, current_y);
                current.push_back(Square(new_size, current_x, current_y));
                current_max_size+=1;
                handler.SetSquare(matrix, n, current_x, current_y, new_size, 1);
                count+=3;
            }
            else{
                finish = current;
                result_size = current_max_size;
                handler.PopBackVector(matrix, n, current, current_max_size, flag);
                count+=2;
            }
        }
        for (int i=0; i<n; i++){
            delete [] matrix[i];
        }
        delete [] matrix;
        count+=2*n+1;
    }

    
    std::cout<<"\nMinimal result size: "<< result_size<<"\nCoordinates & Sizes of result squares: \n";
    handler.PrintVector(finish);

    std::cout<<"\nAll number of operations for square size "<< n <<" = "<<count<<"\n";
    return 0;
}
