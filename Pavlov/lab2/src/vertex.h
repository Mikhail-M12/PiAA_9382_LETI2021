#pragma once
#include <vector>
#include <string>

struct Vertex {
	std::string name;
	Vertex* parent;
	std::vector <std::pair<Vertex*, double>> adjacent;
	double distance, func;
	bool closed;

	Vertex(std::string, double = 0.0);
	Vertex(std::string, Vertex*, double, double = 0.0);

	bool noAdjacent();
	bool cmp(const Vertex&);	// Компаратор приоритетов вершин для PriorQueue
};