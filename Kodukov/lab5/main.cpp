#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <iostream>
#include <algorithm>


//#define TASK1
#define TASK2
#define DEBUG

typedef std::unordered_map<char, int> node;

class AhoKorasik {
 public:
  std::vector<node> bor;
  std::vector<std::vector<int>> term;
  std::vector<int> suff_link;
  int curPos, maxEdges = 0;

  AhoKorasik() {}

  // Initialize machine
  void init(const std::vector<std::string> &words) {
    // Build bor
#ifdef DEBUG
    std::cout << "Building tree for words:"; 
    for (auto &word : words)
      std::cout << "  " << word << "\n";
    std::cout << "\n";
#endif
    int total = 0, word_cnt = 0;
    std::unordered_map<char, int> root;
    this->bor.push_back(root);  // root of tree
    this->term.push_back(std::vector<int>());
    this->suff_link.push_back(0);

    // reading patterns by chars
    for (auto &word : words) {
#ifdef DEBUG
      std::cout << "Current word: " << word << "\n\n";
#endif
      int cur = 0;  // start from root
      for (char c : word) {
#ifdef DEBUG
        std::cout << "Current symbol: " << c << "\n";
#endif
        if (this->bor[cur].find(c) != this->bor[cur].end()) {  // if edge for 'c' found, go there
#ifdef DEBUG
          std::cout << "  This symbol is child of current node, go there\n";
#endif
          cur = this->bor[cur][c];
        } else {  // else add new node
#ifdef DEBUG
          std::cout << "  No child node with this symbol, creating new and go there\n";
#endif
          node newNode;
          this->bor.push_back(newNode);
          this->term.push_back(std::vector<int>());
          total++;
          this->bor[cur][c] = total;
          cur = total;
        }
      }
#ifdef DEBUG
      std::cout << "End of word. Mark last node as terminal\n\n";
#endif
      this->term[cur].push_back(word_cnt++);  // add teminal node for current word
    }

    // All suffix links points to root by default
    for (int i = 0; i < this->bor.size(); i++) {
      this->suff_link.push_back(0);
    }

#ifdef DEBUG
    std::cout << "Creating suffix links...\n";
#endif
    // BFS to find suffix links
    std::queue<std::pair<int, node>> nodes;  // queue for bfs
    maxEdges = this->bor[0].size();
    // filling queue from root
    for (auto kv : this->bor[0]) {
      auto o = make_pair(kv.second, this->bor[kv.second]);
      nodes.push(o);
    }
#ifdef DEBUG
    std::cout << "Current queue: " << std::endl;
    auto printQueue = nodes;
    while (!printQueue.empty()) {
      std::cout << printQueue.front().first << " ";
      printQueue.pop();
    }
    std::cout << "\n";
#endif
    while (!nodes.empty()) {
      // get node from queue
      auto p = nodes.front();
      nodes.pop();
      int i = p.first;
      node curnode = p.second;

      // update maximum edges
      if (maxEdges < curnode.size())
        maxEdges = curnode.size();

#ifdef DEBUG
      std::cout << "Current node: " << i << "\n";
      std::cout << "Suffix link: " << this->suff_link[i] << "\n";
#endif

      // add next nodes to queue
      for (auto kv : curnode) {
        auto pp = make_pair(kv.second, this->bor[kv.second]);
        nodes.push(pp);
      }
      // build suffix links for children
      for (auto kv : curnode) {
        char child = kv.first;
        int pos = kv.second;
        int f = this->suff_link[i];
#ifdef DEBUG
        std::cout << "Current child: " << pos << ", " << child << "\n";
#endif
        // Find nearest suffix link of parents that has edge to child's symbol
        while (f != 0 && this->bor[f].find(child) == bor[f].end()) {
          f = this->suff_link[f];
#ifdef DEBUG
          std::cout << "No such symbol in children of current link " << f << " go futher\n";
          std::cout << "Next link: " << f << "\n";
#endif
        }
        // create suffix link if node was found
        if (this->bor[f].find(child) != bor[f].end()) {
          f = this->bor[f][child];
        }
#ifdef DEBUG
          std::cout << "Create new suffix link: " << pos << " -> " << f << "\n";
#endif
        this->suff_link[pos] = f;
        // Add termination flag for all nodes which suffix chain leads to
        // termination node
        if (!this->term[this->suff_link[pos]].empty()) {
#ifdef DEBUG
        std::cout << "Suffix link was terminal node, add same mark to cuurent child\n";
#endif
          for (auto item : this->term[this->suff_link[pos]]) {
            this->term[pos].push_back(item);
          }
        }
      }
    }
    this->curPos = 0;  // back to root

#ifdef DEBUG
    std::cout << "\n Built automation: \n";
    for (int i = 0; i < bor.size(); i++) {
      std::cout << "Index: " << i << "\n";
      if (!term[i].empty()) {
        std::cout << "Terminal node, indices of words: " << "\n";
        for (auto item : term[i]) {
          std::cout << item << " ";
        }
        std::cout << "\n";
      }
      std::cout << "Suffix link: " << suff_link[i] << "\n";
      if (bor[i].empty()) {
        std::cout << "No children";
      } else {
        std::cout << "Children: ";
        for (auto item : bor[i]) {
          std::cout << "{" << item.first << ", " << item.second << "} ";
        }
      }
      std::cout << "\n\n";
    }
#endif
  }

  // Process one symbol in machine
  std::vector<int> process(char letter) {
#ifdef DEBUG
    std::cout << "Searching symbol " << letter << "\n";
#endif
    // Find the matching position in machine by suffix links.
    while (this->curPos > 0 && this->bor[this->curPos].find(letter) == this->bor[this->curPos].end()) {
      this->curPos = this->suff_link[this->curPos];
#ifdef DEBUG
      std::cout << "No matches for index " << curPos << "\n";
      std::cout << "Go to suffix link " << curPos << "\n";
#endif
    }
    // go to node if it was found
    if (this->bor[this->curPos].find(letter) != this->bor[this->curPos].end()) {
      this->curPos = this->bor[this->curPos][letter];
#ifdef DEBUG
      std::cout << "Symbol found on index " << curPos << "\n";
#endif
    }

    // return all words that end on this node
#ifdef DEBUG
    std::cout << "Return all words which terminates on this node: " << "\n";
    for (auto item : this->term[this->curPos]) {
      std::cout << item << " ";
    }
    std::cout << "\n\n";
#endif
    return this->term[this->curPos];
  }

  //find all patterns in text + cut text
  std::string find_all_words(const std::string &text) {
    int n = 0;
    std::cin >> n;

    std::vector<std::string> words;
    for (int i = 0; i < n; i++) {
      std::string word;
      std::cin >> word;
      words.push_back(word);
    }

    init(words);

    std::vector<std::pair<int, int>> res;
    for (int pos = 0; pos < text.length(); pos++) {
      std::vector<int> pats = process(text[pos]);
      if (!pats.empty()) {
        for (auto point : pats) {
          int patternLength = words[point].length();
          res.emplace_back(std::make_pair(pos - patternLength + 2, point + 1));
        }
      }
    }

    std::string crop = text;

    std::sort(res.begin(), res.end());
    for (auto item : res) {
      for (int i = item.first - 1; i < item.first + words[item.second - 1].size() - 1; i++)
        crop[i] = '-';
      std::cout << item.first << " " << item.second << std::endl;
    }
    for (int i = 0; i < crop.size(); i++) {
      if (crop[i] == '-') {
        crop.erase(crop.begin() + i);
        i--;
      }
    }
    return crop;
  }

  std::string find_with_joker(const std::string &text) {
    std::string word;
    std::cin >> word;
    char joker;
    std::cin >> joker;

    std::vector<int> predictions(text.length(), 0);
    std::vector<std::string> patterns;
    std::vector<int> index;
    int j = 0;
    while (j < word.length()) {
      std::string small;
      int i = j;
      while (j < word.length() && word[j] != joker) {
        small += word[j];
        j++;
      }
      if (!small.empty()) {
        patterns.push_back(small);
        index.push_back(i);
      }
      j++;
    }

    init(patterns);
    
    for (int pos = 0; pos < text.length(); pos++) {
      std::vector<int> pats = process(text[pos]);
      if (!pats.empty()) {
        for (auto point : pats) {
          int firstLetter = pos - patterns[point].length() + 1;
          int idx = firstLetter - index[point] + 1;
          if (idx >= 0 && idx < predictions.size()) {
            predictions[firstLetter - index[point] + 1]++;
          }
        }
      }
    }

    std::string crop = text;
    for (int i = 0; i < predictions.size() - word.size() + 2; i++) {
      if (predictions[i] == patterns.size()) {
        for (int j = i - 1; j < word.size() + i - 1; j++)
          crop[j] = '-';
        std::cout << i << std::endl;
      }
    }

    for (int i = 0; i < crop.size(); i++)
      if (crop[i] == '-') {
        crop.erase(crop.begin() + i);
        i--;
      }
    return crop;
  }
};

int main() {
  std::string seq;

  AhoKorasik instance;
  std::string entr;
#ifdef TASK1
#ifdef DEBUG
  std::cout << "Second task\nInput:\n";
#endif
  std::cin >> seq;
  entr = instance.find_all_words(seq);
#else
#ifdef DEBUG
  std::cout << "Second task\nInput:\n";
#endif
  std::cin >> seq;
  entr = instance.find_with_joker(seq);
#endif

#ifdef DEBUG
  std::cout << "\nString without words: " << entr << "\n";
  std::cout << "Max edges: " << instance.maxEdges << "\n";
#endif

  return 0;
}