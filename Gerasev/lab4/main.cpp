#include "string"
#include "vector"
#include "iostream"

#define FIRSTMODE false
#define COLOR BLUE


#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */

void highlightedLetterPrint(int highlighted, std::string output)
    {
        for (int i=0; i<output.length(); i++)
        {
            if (i == highlighted) {
                std::cout << COLOR << output[i] << RESET;
            } else {
                std::cout << output[i];
            }
        }
    }


void printStringsHandelingState(std::string & first, std::string & second, int firstIndex, int secondIndex)
{
    highlightedLetterPrint(firstIndex, first);
    std::cout << " | ";
    highlightedLetterPrint(secondIndex, second);
    std::cout << '\n';
}


std::vector<int> getPrefixFunction(std::string & pattern) {
    auto prefixFunction = std::vector<int>(pattern.length());

    int i = 1;
    int j = 0; // Indexes
    
    std::cout << "Creating prefix function\n";

    while (i != pattern.length()) {

        std::cout << "i = " << i << "; j = " << j << "\n";

        if (pattern[i] == pattern[j]) {
            // If symbols are equal -- prefix function of i is function[j+1]
            printStringsHandelingState(pattern, pattern, j, i);
            std::cout << "Symbols are equal, so f[" << i << "] = " << j + 1 << ", i and j are inсreased by 1\n\n";
            prefixFunction[i] = j + 1;
            i++;
            j++;
        } else if (j == 0) {
            // In this case f[i] = 0
            printStringsHandelingState(pattern, pattern, j, i);
            std::cout << "Symbols are not equal and j == 0 => f["
                << i << "] = 0" << ", i inсreased by 1\n\n";

            prefixFunction[i] = 0;
            i++;
        } 
        else {
            // In this case we change j on prefixFunction[j - 1]
            printStringsHandelingState(pattern, pattern, j, i);
            std::cout << "Symbols are not equal and j != 0 => j = " << prefixFunction[j - 1] << "\n\n";
            j = prefixFunction[j - 1];
        }
    }
    return prefixFunction;
}


std::vector<int> getSubstringIndexes(std::string & text, std::string & pattern) {
    auto prefixFunction = getPrefixFunction(pattern);

    int textIndex = 0;
    int substringIndex = 0;
    auto result = std::vector<int>();

    std::cout << "[Looking for pattern in the text]" << '\n';

    // While there is any text left
    while (textIndex != text.length()) {

        std::cout << "textIndex = " << textIndex << "; substringIndex = " << substringIndex << "\n";

        if (text[textIndex] == pattern[substringIndex]) {
            textIndex += 1;
            substringIndex += 1;

            if (substringIndex == pattern.length()) {
                std::cout << "Symbols are equal and there is no symbols in the substring left\n";
                printStringsHandelingState(text, pattern, textIndex - 1, substringIndex - 1);
                result.push_back(textIndex - substringIndex);
                substringIndex = prefixFunction[substringIndex - 1];
            } else {
                std::cout << "Symbols are equal and there are symbols in the substring left, handeling them...\n";
                printStringsHandelingState(text, pattern, textIndex - 1, substringIndex - 1);
            }

        } else { // If symbols are not equal
            if (substringIndex == 0) {
                std::cout <<"Symbols are not equal and this is a first letter of substring\n";
                printStringsHandelingState(text, pattern, textIndex, substringIndex);
                textIndex += 1;
                
            }
            // If it's not a first symbol -- changing substringIndex to prefixFunction[substringIndex - 1]
            else {
                std::cout << "Symbols are not equal and this is not a first letter of substring => it's now equal prefixFunction[substringIndex - 1]\n";
                printStringsHandelingState(text, pattern, textIndex, substringIndex);
                substringIndex = prefixFunction[substringIndex - 1];
            }
        }
    }

    std::cout << "There is no symbols left in the text\n";

    return result;
}


void firstTask(std::string & text, std::string & pattern) {
    auto result = getSubstringIndexes(text, pattern);

    if (!result.empty()) {
        for (int i = 0; i < result.size() - 1; ++i) std::cout << result[i] << ',';
        std::cout << result[result.size() - 1];
    } else {
        std::cout << -1;
    }
}


void secondTask(std::string & text, std::string & pattern) {

    if (text.length() != pattern.length()) {
        std::cout << -1;
        return;
    }

    text = text + text;
    auto result = getSubstringIndexes(text, pattern);
    if (!result.empty()) { 
        std::cout << result[0]; 
    } else {
        std::cout << -1;
    }
}


int main() {
    std::string text;
    std::string pattern;

    std::cin >> text;
    std::cin >> pattern;

    if (FIRSTMODE) {
        firstTask(text, pattern);
    } else {
        secondTask(text, pattern);
    }
    std::cout << '\n';
    return 0;
}