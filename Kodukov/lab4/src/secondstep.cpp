#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//#define DEBUG

// Knuth–Morris–Pratt algorithm
long long KMP(const std::string &str, const std::string &pattern) {
  long long i, k, len;
  len = pattern.length();

  std::vector<long long> d, res;
  d.resize(len + 1);

  // Prefix-function of pattern
#ifdef DEBUG
  std::cout << "***Computing prefix-function for pattern***\n\n";
#endif
  d[0] = 0; //pi(str, 0) = 0 by definition
  for (i = 1, k = 0; i < len; i++) { 
#ifdef DEBUG
    std::cout << "i = " << i << " k = " << k << " ";
    std::cout << "S[" << i << "] = " << pattern[i];
#endif
    // decrease k back if symbols aren't equal
    while (k > 0 && pattern[k] != pattern[i]) {
#ifdef DEBUG
      std::cout << " != " << "S[" << k << "] = " << pattern[k] << " -> pf[i] = k = pf[k - 1]\n";
#endif
      k = d[k - 1];
    }
    // increase k if symbols are equal
    if (pattern[k] == pattern[i]) {
#ifdef DEBUG
      std::cout << " == " << "S[" << k << "] = " << pattern[k] << " -> pf[i] = k + 1\n";
#endif
      k++;
    } else {
#ifdef DEBUG
      std::cout << "\n";
#endif
    }
    // save prefix-function for position i
    d[i] = k;
  }

#ifdef DEBUG
  std::cout << "***Searching cyclic shift***\n\n";
#endif
  // Search in text
  for (i = 0, k = 0; i < str.length(); i++) {
    // decrease k back if symbols aren't equal
#ifdef DEBUG
    std::cout << "i = " << i << " k = " << k << " ";
    std::cout << "S[" << i << "] = " << str[i];
#endif
    while (k > 0 && pattern[k] != str[i]) {
#ifdef DEBUG
      std::cout << " != "
                << "S[" << k << "] = " << pattern[k]
                << " -> pf[i] = k = pf[k - 1]\n";
#endif
      k = d[k - 1];
    }
    // increase k if symbols are equal
    if (pattern[k] == str[i]) {
#ifdef DEBUG
      std::cout << " == "
                << "S[" << k << "] = " << pattern[k] << " -> pf[i] = k + 1\n";
#endif
      k++;
    } else {
#ifdef DEBUG
      std::cout << "\n";
#endif
    }
    // suffix equal to pattern was found 
    if (k == len) {
#ifdef DEBUG
      std::cout << "k = " << k << " == length of pattern, cyclic shift was found\n";
#endif
      return i - k + 1;
    } 
  }
  return -1;
}

int main() {
  std::string str, pattern;

  // get strings
  std::getline(std::cin, pattern);
  std::getline(std::cin, str);
  // cyclic shift must be same length
  if (str.length() != pattern.length()) {
    std::cout << "-1";
  } else {
    // search pattern in doubled text to find cyclic shift
    long long res = KMP(str + str, pattern);
    std::cout << res;
  }
  return 0;
}