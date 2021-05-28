#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <sstream>
#include <algorithm>
#define ALPHABET_LEN 5

class BohrVertex{
public:
	int parent;
	char value;
    int next_ver[ALPHABET_LEN];
	std::vector <int> num_pattern;	
	bool flag = false; // является ли шаблоном
	int suffix_link = -1; // суффиксная ссылка от этой вершины
	int move[ALPHABET_LEN];
    BohrVertex(int, char);
	BohrVertex() = default;
    ~BohrVertex() = default;
};

class Bohr{
	std::vector <BohrVertex> bohr;
	std::vector < std::string > pattern;
	int get_move(int v, int edge);
	int get_suffix_link(int v);
	
public:	
	Bohr();
	~Bohr() = default;
	void add_in_bohr(std::map<char, int> m, std::string s);
	void find_matches(std::map<char, int> m, std::string &s, std::vector <int> & array, const std::vector <int> & len);
	void res(int v, int i, std::vector <int> &array, std::vector <int> len);
	std::vector <int> patterns(std::map<char, int> m, std::stringstream& string_pattern, char joker);
	void print_res(const std::vector<int>& array, int t_size, int length);
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
	num_pattern.resize(0);
}

// Добавление образца в бор
void Bohr::add_in_bohr(std::map<char, int> m, std::string s){
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
	pattern.push_back(s);
	bohr[n].num_pattern.push_back(pattern.size() - 1);
	
}

// Получаем суффиксную ссылку
int Bohr::get_suffix_link(int v){
	if (bohr[v].suffix_link == -1)
		if (v == 0 || bohr[v].parent == 0) // корень указывает на себя
			bohr[v].suffix_link = 0;
	else 
		bohr[v].suffix_link = get_move(get_suffix_link(bohr[v].parent), bohr[v].value);
	return bohr[v].suffix_link;
}

// перемещаемся по бору по ребру edge
int Bohr::get_move(int v, int edge){
    if (bohr[v].move[edge] == -1)
        if (bohr[v].next_ver[edge] != -1)
         	bohr[v].move[edge] = bohr[v].next_ver[edge];
        else
			if (v == 0)
				bohr[v].move[edge] = 0;
			else
				bohr[v].move[edge] = get_move(get_suffix_link(v), edge);
   return bohr[v].move[edge];
}

void Bohr::res(int v, int i, std::vector <int>& array, std::vector <int> len){
	 for(int u = v;u != 0;u = get_suffix_link(u))
        if (bohr[u].flag){
			for (const auto &j : bohr[u].num_pattern)
				if ((i - len[j] < array.size())) 
					array[i - len[j]]++;
		}
}

std::vector <int> Bohr::patterns(std::map<char, int> m, std::stringstream& string_pattern, char joker){
	std::vector < int > len;
	int length = 0;
	std::string buffer;
	while (getline(string_pattern, buffer, joker)){
		if (buffer.size() > 0){
			length += buffer.size();
			len.push_back(length);
			add_in_bohr(m, buffer);
		}
		length++;
	}
	return len;
}

void Bohr::print_res(const std::vector<int>& array, int t_size, int length){

	for (int i = 0; i < t_size; i++)
		if ((array[i] == pattern.size()) && (i + length <= t_size)){
			std::cout << i + 1 << "\n";
		}
	
}

void Bohr::find_matches(std::map<char, int> m, std::string &s, std::vector <int> & array, const std::vector <int> & len){
	int edge;
	int u = 0;
	int lenght = s.length();
	for (int i = 0; i < lenght; i++){
		char symb = s[i];
		edge = m[symb];
        u = get_move(u, edge);
        res(u, i + 1, array, len);
    }
}
	
int main(){
	std::map<char, int> m { {'A', 0}, {'C', 1}, {'G', 2}, {'T', 3}, {'N', 4}}; // буква и её значение
	Bohr bohr;
	std::string text, pattern_text;
	char joker;
	std::cin >> text >> pattern_text >> joker;
	std::stringstream str_stream(pattern_text);
	std::vector <int> len = bohr.patterns(m, str_stream, joker);
	std::vector <int> array(text.length(), 0);
	bohr.find_matches(m, text, array, len);
	bohr.print_res(array, text.size(), pattern_text.length());
}