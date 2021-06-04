#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <algorithm>
#define ALPHABET_LEN 5
std::vector <std::pair<int, int>> output;

class BohrVertex{
public:
	int parent;//вершина родитель
	char value;
	int next_ver[ALPHABET_LEN]; //массив, где next_ver[i] - номер вершины, в которую мы перейдём по символу с номером i в алфавите
	int numPattern = 0; //номер строки-шаблона для вершины next_ver[i]
	bool flag = false; // является ли шаблоном
	int suffLink = -1; // суффиксная ссылка от этой вершины
	int move[ALPHABET_LEN];
	BohrVertex(int, char);
	BohrVertex() = default;
	~BohrVertex() = default;
};

class Bohr{
	std::vector <BohrVertex> bohr;//вершины бора
	int get_move(int v, int edge);
	int get_suffix_link(int v);
	void res(std::vector <std::string>& patterns, int v, int i, std::string text);
	
public:	

	Bohr();
	~Bohr() = default;
	void add_in_bohr(std::map<char, int> m, int index, std::string s);
	void find_matches(std::vector <std::string> patterns, std::string s, std::map<char, int> m);
	
};

Bohr::Bohr(){
	bohr.push_back(BohrVertex(0,0));
}

BohrVertex::BohrVertex(int parent, char symbol){
	for (int i = 0; i < ALPHABET_LEN; i++){
		next_ver[i] = -1;
		move[i] = -1;
	}
	this->parent = parent;
	this->value = symbol;
}

// добавляем образец в бор
void Bohr::add_in_bohr(std::map<char, int> m, int index, std::string s){
	int n = 0;
	int edge = 0;
	int len =  s.length();
	for (int i = 0; i < len; i++){
		char symb = s[i];
		edge = m[symb];//вычисляем номер символа
		if (bohr[n].next_ver[edge] == -1){	// нет ребра
			bohr.push_back(BohrVertex(n, edge));//добавляем ребро
			bohr[n].next_ver[edge] = bohr.size() - 1;
		}
		n = bohr[n].next_ver[edge];
	}
	bohr[n].flag = true;
	bohr[n].numPattern = index;
}

// получаем суффиксную ссылку
int Bohr::get_suffix_link(int v){
	if (bohr[v].suffLink == -1)
		if (v == 0 || bohr[v].parent == 0) // корень указывает на себя
			bohr[v].suffLink = 0;
	else 
		bohr[v].suffLink = get_move(get_suffix_link(bohr[v].parent), bohr[v].value);
	return bohr[v].suffLink;
}

// перемещаемся по бору по ребру edge
int Bohr::get_move(int v, int edge){
	if (bohr[v].move[edge] == -1)//если из текущей вершины нельзя переместиться
		if (bohr[v].next_ver[edge] != -1)//пробуем переместиться из следующей вершины
			bohr[v].move[edge] = bohr[v].next_ver[edge];
		else
			if (v == 0)//если корень
				bohr[v].move[edge] = 0;
			else
				bohr[v].move[edge] = get_move(get_suffix_link(v), edge);//если не корень перемещаемся по суффиксной ссылке
	return bohr[v].move[edge];
}

void Bohr::res(std::vector <std::string>& patterns, int v, int i, std::string text){
	for(int u = v; u != 0; u = get_suffix_link(u)){
		if (bohr[u].flag)
			output.push_back(std::make_pair(i - patterns[bohr[u].numPattern].length() + 1, bohr[u].numPattern + 1));
	}
}

//находим шаблоны в тексте
void Bohr::find_matches(std::vector <std::string> patterns, std::string s, std::map<char, int> m){
	int u = 0;
	int edge;
	int len = s.length();
	for (int i = 0; i < len; i++){
		char symb = s[i];
		edge = m[symb];
		u = get_move(u, edge);//перешли из u по ребру edge
		res(patterns, u, i + 1, s);
	}
}
	
int main(){
	std::map<char, int> m { {'A', 0}, {'C', 1}, {'G', 2}, {'T', 3}, {'N', 4}}; // буква и её значение
	Bohr bohr;
	std::vector <std::string> patterns;
	std::string text;
	int number = 0;
	std::string pattern;
	std::cin >> text >> number;
	for (int i = 0; i < number; i++){
		std::cin >> pattern;
		bohr.add_in_bohr(m, i, pattern);
		patterns.push_back(pattern);
	}
	bohr.find_matches(patterns, text, m);
	sort(output.begin(), output.end());
	for (std::vector <std::pair<int, int>>::iterator it = output.begin(); it!=output.end(); ++it)
		if (true)
			std::cout<< (*it).first << ' ' << (*it).second << "\n";
	return 0;
}
