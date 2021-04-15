#include "vertex.h"

Vertex::Vertex(std::string s, double dist) : name(s), distance(dist), func(0.0), parent(nullptr), closed(false) {}

Vertex::Vertex(std::string s, Vertex* inc, double w, double dist) : name(s), distance(dist), func(0.0), parent(nullptr), closed(false) {
	adjacent.push_back({ inc, w });
}

bool Vertex::noAdjacent(){
	for (auto& v : adjacent) {
		if (!v.first->closed) return false;
	}
	return true;
}

bool Vertex::cmp(const Vertex& other) {
	if (this->func == other.func) {
		return this->name > other.name;
	}
	return (this->func) < (other.func);
}