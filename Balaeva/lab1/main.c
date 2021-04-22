#include <iostream>
#include <vector>
#include <ctime>

int bestNum = 0;

class Table // исходный квадрат
{
    int size;
    std::vector<std::vector<int>> table;
    int count;

public:
    Table(int size): size(size), table(size, std::vector<int>(size, 0)), count(0) {
        if(size != 0) {
            constTable();
        }
    }
    Table(Table const &other): size(other.size), table(other.size, std::vector<int>(other.size, 0)), count(other.count) {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                table[i][j] = other.table[i][j];
    }
    Table& operator=(Table const &other){
        if(&other != this){
            Table tmp(other);
            count = tmp.count;
            size = tmp.size;
            table.swap(tmp.table);
        }
        return *this;
    }
    ~Table(){}

    void constTable(){ //Вставка трех "гарантированных " квадратов
        int temp = size/2;
        insertTable(0, 0, temp + 1);
        insertTable(0, temp + 1, temp);
        insertTable(temp + 1, 0, temp);
    }

    int getnumber(){
        return count;
    }
    int getsize(){
        return size;
    }

    bool isPossible(int i, int j, int n){
        if((i + n) > size || (j + n) > size){
            return false;
        }
        for(int y = i; y < i + n; y++)
            for (int x = j; x < j + n; x++)
                if(table[y][x] != 0){
                    return false;
                }
        return true;
    }
    void insertTable(int i, int j, int n){
        for(int y = i; y < i + n; y++){
            for (int x = j; x < j + n; x++){
                table[y][x] = n;
            }
        }
        ++count;
    }
    bool checkSpace(int i){
        for(int y = i; y < size; y++)
            for (int x = 0; x < size; x++)
                if(table[y][x] == 0)
                    return true;
        return false;
    }
    int findi(int i){
        for (int y = i; y < size; y++)
            for (int x = 0; x < size; x++)
                if (table[y][x] == 0){
                    return y;
                }
    }
    int findj(int i){
        for (int y = i; y < size; y++)
            for (int x = 0; x < size; x++)
                if (table[y][x] == 0){
                    return x;
                }
    }
    void deleteTable(int i, int j){
        int val = table[i][j];
        for (int y = i; y < i + val; y++)
            for (int x = j; x < j + val; x++)
                table[y][x] = 0;
    }
    void result(){
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(table[i][j] != 0){
                    std::cout << i + 1 << " " << j + 1 << " " << table[i][j] << std::endl;
                    deleteTable(i, j);
                }
            }
        }
    }

};

Table best(0);

Table backTracking(Table table, int i, int j){
    for(int n = table.getsize() / 2; n > 0; n--){
        if(table.getnumber() > bestNum){
            return table;
        }

        Table shape = table;
        if(shape.isPossible(i, j, n)){
            shape.insertTable(i, j, n);
            if(shape.checkSpace(i)){
                shape = backTracking(shape, shape.findi(i), shape.findj(i));
            }
            else if(bestNum >= shape.getnumber()){
                best = shape;
                bestNum = shape.getnumber();
            }
        }
    }
    return table;
}

int main(){
    int size = 0;
    std::cin >> size;
    clock_t start = clock();
    if (size%2 == 0){
        int temp = size/2;
        std::cout << "4" << std::endl;
        std::cout << "1 1 " << temp << std::endl;
        std::cout << temp+1 << " 1 " << temp << std::endl;
        std::cout << "1 " << temp+1 << " " << temp << std::endl;
        std::cout << temp+1 << " " << temp+1 << " "<< temp << std::endl;

    }
    else if (size%3 == 0){
        int temp = size/3;
        std::cout << "6" << std::endl;
        std::cout << "1" << " 1 " << temp*2 <<  std::endl;
        std::cout << "1 " << 1+temp*2 << " " << temp <<  std::endl;
        std::cout << 1+temp << " " << 1+temp*2 << " " << temp <<  std::endl;
        std::cout << 1+temp*2 << " 1 "  << temp <<  std::endl;
        std::cout << 1+temp*2 << " " << 1+temp << " "  << temp <<  std::endl;
        std::cout << 1+temp*2 << " " << 1+temp*2 << " " << temp << std::endl;

    }
    else {
        bestNum = size * size;
        Table A(size);
        A = backTracking(A, A.findi(0), A.findj(0));
        std::cout << bestNum << std::endl;
        best.result();
    }
    clock_t end = clock();
    std::cout << "Время выполнения: " << (double) (end - start) / CLOCKS_PER_SEC << "\n";
return 0;
}
