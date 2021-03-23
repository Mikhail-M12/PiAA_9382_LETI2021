#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <chrono>
#include <cmath>
#define debugdetails 0
int min(int a , int b);
class Grid {

public:
    std::vector<std::vector<bool>> m;
  	int n;

    Grid(int n) :n(n){
        for (int i = 0; i < n; i++) {
            std::vector<bool> a;
            for (int j = 0; j < n; j++)
                a.push_back(0);
            m.push_back(a);
        }

    }
    bool operator()(int i, int j) {
        return *((m.begin() + i)->begin() + j);
    }
    void set(int i, int j, int value) {
        *((m.begin() + i)->begin() + j) = value;
    }
    void print() {
        for (auto it : m) {
            for (auto it2 : it)
                std::cout << it2 << " ";
            std::cout << "\n";
        }

    }
    Grid operator=(Grid one){
    	for (int i = 0 ; i < n; i++)
    		for (int j = 0 ; j < n; j++)
    			set(i,j, one(i,j));
    		
    }
};
class Square {

public:
    int x, y;
    int size;
    Square(int y, int x, int size){

    	this->size = size;
    	this->x = x;
    	this->y = y;

    } 
    Square() {}
    void print() {
        std::cout << x + 1 << " " << y + 1 << " " << size << "\n";
    }
    friend bool operator==(Square s1, Square s2) {
        if (s1.x == s2.x && s1.y == s2.y && s1.size == s2.size) return true;
        else return false;
    }
    std::vector<Square> squaresUsedOnCurrentStep;

};
class SquareContainer {
public:
    Grid* grid;
    int n;
    int sum;
    std::stack<Square>squares;
    SquareContainer(int n) {
        this->n = n;
        grid = new Grid(n);
        sum = 0;
    }
    void push(Square a) { //функция для вставки в стэк квадрата и изменения матрицы заполненности
        for (int i = a.y; i < a.y + a.size; i++)
            for (int j = a.x; j < a.x + a.size; j++){
                grid->set(i, j, 1);     
 				
 			}
 		if (squares.size() > 1)squares.top().squaresUsedOnCurrentStep.push_back(a);	
        squares.push(a);
        #ifdef debugdetails
         	   std::cout<<"Поставлен квадрат: ";
         	   squares.top().print();
        #endif
        sum += a.size * a.size;
    }
    void pop() { //функция для удаления последнего квадрата из стэка и изменения матрицы заполненности

    	for (int i = squares.top().y; i < squares.top().y + squares.top().size; i++)
            for (int j = squares.top().x; j < squares.top().x + squares.top().size; j++){
                grid->set(i, j, 0);            
            }
             #ifdef debugdetails
                std::cout<<"Удален квадрат ";
                squares.top().print();
            #endif
            sum -= squares.top().size * squares.top().size;
            squares.pop();
    }
    bool isFull() {
        if (sum == n*n) return true;
        else return false;

    }
    bool isPossibleToPlace(Square s, std::vector<Square> used){
    	int i = s.y;
    	int j = s.x;
    	int size = s.size;
    	int intersection = false;
    	
     	if (size <= min(n - i, n - j)){    //пересечение и подходящий размер
            for (int h = i; h < i + size; h++){
                for (int b = j; b < j + size; b++)
                    if ((*grid)(h, b)) {
                        intersection = true;
                        return false;
                    }
            }
        
            for (auto it : used){
            					//использован ранее
                if (it == s)
                    return false;
            }
            return true;     
        }
        else return false;
	}
};
int min(int a, int b) {
    if (a < b) return a;
    else return b;
}
Square generateNewSquare(SquareContainer* a,  std::vector<Square>& used, int maxsize) { //расширение частичного ршения
    int n = a->n;
    int size = maxsize;
    int row = 0;
    int column = 0;
    bool breakflag = false;
    for (int i = 0; i < n; i++){ 									//поиск первой незанятой клетки
        for (int j = 0; j < n; j++){
        		if (!(*a->grid)(i,j)){
        			row = i;
        			column = j;
        			breakflag = true;
        			break;
        		}	
        }
        if(breakflag)break;
    }

        		int minsize = 1;
        		while (size >= minsize){
            		Square tmp(row,column,size);	
         			if (a->isPossibleToPlace(tmp, used)){
         				return tmp;
         			}
         			else{
         				if (size == minsize){
                             #ifdef debugdetails
                                    std::cout<<"На данном шаге были просмотрены все варианты. Не удается найти новых\n";
                                    //std::cout<<"Откат на один шаг\n";
                                #endif
                              return Square(-1, -1, -1);
                        }
                         
         				else size -= 1;
         		    }
         	    }   
         	return Square(-1, -1, -1);
    }
bool isprime(int n) {
    int i = 2;
    while (i <= sqrt(n)) {
        if (n % i == 0) return false;
        i++;
    }
    return true;
}
bool issquare(int n){
	int i = 0;
	for (i; i <= sqrt(n); i++)
		if (n == i * i)return true;
	return false;
}
void doSquaring(int  n){
	int min = sqrt(n);
    int size = n - 1;
    int index = 1;
    bool flg = false;
    if (n % 2 == 0){ //оптимизация дле четных n
        size = n / 2;
        min = 4;
    }
    else if (n % 3 == 0){ //оптимизация для n делящихся на 3
        size = (n / 3) * 2;
        min = 6;
    }
    else if (!isprime(n)) {
        //n = sqrt(n);
        int maxdivide = n;
        int i = n - 1;
        while (i > 0) {
            if (n % i == 0) {
                maxdivide = i;
                break;
            }
            i--;
        }
        size = ceil((double)maxdivide / 2) * maxdivide;
        min = sqrt(n);

    }
    else if (n > 3) { 
        min = sqrt(n) + 2;
        size = n - n / 2;
        index = 2;
        flg = true;

    }
    int count = 0;
    SquareContainer* a = new SquareContainer(n);
    #ifdef debugdetails
        std::cout<<"Постановка 3х начальных квадратов\n";
    #endif
    a->push(Square(0,0,size));
   	a->push(Square(size, 0, n - size));
    a->push(Square(0, size, n - size));
    if (flg) size = size/2 + 1;
    auto start = std::chrono::system_clock::now();
    for (min; ;min += 1){
        #ifdef debugdetails
                std::cout<<"\nПостроение решения для размера "<<min<<std::endl;
        #endif
        while (1) {
            if (!a->isFull()){
            	count++;
                if (a->squares.size() == min){ //если размер уже максимальный
                    #ifdef debugdetails
                    std::cout<<"Достигнут максимальный размер(" << min << "), откат назад для поиска лучшего решения\n";
                    #endif
                	if (min > 4)a->pop();
                	 a->pop();
                }
                else {
                    Square squareGenerationResult;
                    squareGenerationResult = generateNewSquare(a, a->squares.top().squaresUsedOnCurrentStep, size); //генерация квадрата
                   
                    if (squareGenerationResult.size != -1){
                        a->push(squareGenerationResult);
                    }
                    else{ 
                    	if (a->squares.size() == 3){
                    	    #ifdef debugdetails
                    	            std::cout<<"Не удалось найти решение для заданного размера - " << min <<std::endl;
                    	    #endif
                    	    
                    		break;
                    	}
                    	else{
                    	  #ifdef debugdetails
                    	        std::cout<<"Откат на один шаг\n";
                    	  #endif
                    	  a->pop();  
                    	} 
                    	
               		}
            	}
        	}
            else break;
        }
        if (a->squares.size() != 3){ //нашлось квадрирование
            #ifdef debugdetails
            std::cout<<"Удалось найти решение для размера " << min <<std::endl;
            #endif
        	break; 
        }    
        
        else                            //обновляем список использованных на первом шаге
        	a->squares.top().squaresUsedOnCurrentStep = std::vector<Square>(); 
    

    }
    auto end = std::chrono::system_clock::now();
    
    std::chrono::duration<double> elapsed_seconds = end - start;
    //std::cout<<"Исходный размер: " << n <<"\nВремя: " << elapsed_seconds.count()<<" операций: " << count << " Результат: "<<a->squares.size()<< "\n";
   // std::cout<<n<<" "<<elapsed_seconds.count()<<" " <<count<<" "<<a->squares.size()<< "\n";
    std::cout<<"Кол-во квадратов: " <<a->squares.size()<< "\n";
    
    while(!a->squares.empty()){
        a->squares.top().print();
        a->squares.pop();
    }

}
int main ()
{

    int n = 2;
    setlocale(LC_ALL, "Russian");
    std::cout<<"Введите размер: ";
    std::cin>>n;
   
    while (n < 2){
        std::cout<<"Введите размер >= 2\n";
        std::cin>>n;
    }
    doSquaring(n);
    
    return 0;
}
