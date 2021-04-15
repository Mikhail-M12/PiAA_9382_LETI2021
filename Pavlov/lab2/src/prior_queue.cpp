#include "prior_queue.h"

void PriorQueue::heapify(int i) {
	int least = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	if (l < heap.size() && heap.at(l)->cmp(*heap.at(least))) {
		least = l;
	}
	if (r < heap.size() && heap.at(r)->cmp(*heap.at(least))) {
		least = r;
	}
	if (least != i) {
		std::swap(heap.at(i), heap.at(least));
		heapify(least);
	}
}

void PriorQueue::stabilize() {
	for (int i = heap.size() / 2 + 1; i >= 0; i--) {
		heapify(i);
	}
}

PriorQueue::PriorQueue(Vertex* initial) {
	heap.push_back(initial);
}

void PriorQueue::push(Vertex* elem) {
	heap.push_back(elem);
}

Vertex* PriorQueue::top() {
	if (heap.empty()) return nullptr;
	stabilize();
	return heap.at(0);
}

void PriorQueue::pop() {
	if (heap.empty()) return;
	std::swap(heap.at(0), heap.at(heap.size() - 1));
	heap.resize(heap.size() - 1);
}