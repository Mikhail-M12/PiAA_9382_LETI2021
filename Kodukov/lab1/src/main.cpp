#include <ctime>
#include <iostream>
#include <vector>
#include <cmath>

#define DEBUG

// map to test packaging
bool map[40][40];

// square to place on a map
struct square {
  int size, x_corner, y_corner;
};

// stack of square placing iterations
typedef std::vector<square> step_stack;
 
// current square placing stack
step_stack curstack;
// best packing
step_stack best;
 
// do step back on stack and delete last square from map
square StepBack(const int &size) {
  if (curstack.size() == 0) return square{-1, -1, -1};
 
  // get last placed square and do step back
  square lastsq = curstack.back();
  curstack.pop_back();
#ifdef DEBUG
  std::cout << "Deleted: " << lastsq.x_corner + 1 << " " << lastsq.y_corner + 1 << " "
            << lastsq.size<< "\n";
#endif
  // clear last placed square
  for (int i = 0; i < lastsq.size; ++i)
    for (int j = 0; j < lastsq.size; ++j)
      map[i + lastsq.y_corner][j + lastsq.x_corner] = 0;
 
  return lastsq;
}
 
// find free cell in square map
square FindFreeCell(int mapsize) {
  for (int i = 0; i < mapsize; ++i)
    for (int j = 0; j < mapsize; ++j)
      if (map[i][j] == 0) return {0, j, i};
  // map is full
  return {-1, -1, -1};
}
 
// place one square on map using last deleted square position
square PlaceSquare(int mapsize, square prediction) {
  square res;
  // use last deleted square information
  if (prediction.size > 0) {
    res = prediction;
    res.size--;
  } 
  else  // first step
    res = {mapsize - 1, 0, 0};
 
  // find size (less than size of previous and fits in map)
  int ressize = 1;
  while (ressize < res.size && 
          (res.x_corner + ressize) < mapsize &&
          (res.y_corner + ressize) < mapsize &&
          map[res.y_corner][res.x_corner + ressize] == 0)
    ressize++;
  res.size = ressize;
  // place square
  for (int i = 0; i < res.size; ++i)
    for (int j = 0; j < res.size; ++j)
      map[res.y_corner + i][res.x_corner + j] = 1;
 
  return res;
}
// find best packing with iterative backtracking
void Backtracking(int size) {
  bool step_back = false, full = false;
  int steps = 3, dividers = 1;
  square prediction{-1, 0, 0}, place_res{0, 0, 0};
  // redude map size to the smallest prime divider
  for (int i = 2; i <= size; i++) {
    if (size % i == 0) {
      dividers = size / i;
      size = i;
    }
  }
#ifdef DEBUG
  std::cout << "N reduced to the smallest prime divider, N = " << size << "\n";
#endif
  // place 3 first squares
#ifdef DEBUG
  std::cout << "Placing 3 first squares:\n";
  std::cout << "Placed square: 1 1 " << (size + 1) / 2 << "\n";
  std::cout << "Placed square: 1 " << (size + 1) / 2 + 1 << " " << size - (size + 1) / 2 << "\n";
  std::cout << "Placed square: " << (size + 1) / 2 + 1 << " 1 " << size - (size + 1) / 2 << "\n";
#endif
  place_res = PlaceSquare(size, {(size + 1) / 2 + 1, 0, 0});
  curstack.push_back(place_res);
  place_res = PlaceSquare(size, {size - (size + 1) / 2 + 1, 0, (size + 1) / 2});
  curstack.push_back(place_res);
  place_res = PlaceSquare(size, {size - (size + 1) / 2 + 1, (size + 1) / 2, 0});
  curstack.push_back(place_res);
  prediction = FindFreeCell(size); prediction.size = size - 1;
  
#ifdef DEBUG
  std::cout << "Start cycle:\n";
#endif
  // check all necessary variants 
  do {
    int last_size = 0;

    full = false;
    step_back = false;

    // while map isn't full
    while (!full) {
      steps++;
      // place one square
      place_res = PlaceSquare(size, prediction);
#ifdef DEBUG
      std::cout << "Placed square: " << place_res.x_corner + 1 << " " << place_res.y_corner + 1 << " " << place_res.size << "\n";
#endif
      curstack.push_back(place_res);
      // go back if current result is worse then best
      if (best.size() > 0 && curstack.size() > 0 &&
          curstack.size() >= best.size()) {
        step_back = true;
#ifdef DEBUG
        std::cout << "Best packaging squares number was reached, go back:\n";
#endif
        StepBack(size);
        prediction = StepBack(size);
        break;
      }
      // prepare to next placing
      prediction = FindFreeCell(size);
      full = (prediction.size == -1);
      prediction.size = size;
    }
    if (!step_back) {
#ifdef DEBUG
    std::cout << "Map is full, current best packaging : " << best.size() << 
      " squares\nComparing with current packaging...\n";
#endif
      // compare with best result
      if (!step_back && curstack.size() < best.size() || best.size() == 0) {
        best = curstack;
#ifdef DEBUG
        std::cout << "Best packaging set to: " << best.size() << " squares\n";
#endif
      }
      else {
#ifdef DEBUG
      std::cout << "Best packaging wasn't changed\n";
#endif
      }
    }
    // go back once and while squares size = 1 (or 2 if map was full)
    do {
      prediction = StepBack(size);
    } while (prediction.size == 1 || (full && prediction.size == 2) && curstack.size() >= 3);
  } while (curstack.size() >= 3);
  // scale results to start map size
  for (int i = 0; i < best.size(); i++) {
    best[i].x_corner *= dividers;
    best[i].y_corner *= dividers;
    best[i].size *= dividers;
  }
  std::cout << "\nTotal placed squares: " << steps << "\n";
}
 
int main() {
  int size;
 
  std::cin >> size;
 
  double t0 = clock();
  if (size % 2 == 0) {
#ifdef DEBUG
    std::cout << "N is even, backtracking isn't required\n";
#endif
    std::cout << 4 << "\n";
    std::cout << 1 << " " << 1 << " " << size / 2 << "\n";
    std::cout << size / 2 + 1 << " " << 1 << " " << size / 2 << "\n";
    std::cout << 1 << " " << size / 2 + 1 << " " << size / 2 << "\n";
    std::cout << size / 2 + 1 << " " << size / 2 + 1 << " " << size / 2 << "\n";
  } else {
#ifdef DEBUG
    std::cout << "N is odd, starting backtracking...\n";
#endif
    Backtracking(size);
    std::cout << best.size() << "\n";
    for (auto &sq : best)
      std::cout << sq.x_corner + 1 << " " << sq.y_corner + 1 << " " << sq.size
                << "\n";
  }
 
  std::cout << "runtime = " << (clock() - t0) / 1000.0 << std::endl;
  system("pause");
  return 0;
}