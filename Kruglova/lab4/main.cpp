#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;

class FullString
//класс хранит текст, шаблон и значения префиксной функции
{
private:
    vector<int> pf; //массив значений префикс функции
    string pattern;  
    string S;
public:
    FullString();
    vector<int> KMP_search();
    void isCyclShift();
    void fill_pf();
};

FullString::FullString()
{
    getline(cin, pattern);
	getline(cin, S);
    pf.resize(pattern.length());
    fill_pf();
}

void FullString::fill_pf()
//заполняем массив значений префиксной функции
{
    std::cout << "Fill pref. fun" << std::endl;
    pf[0] = 0;
    std::cout << "Curr elem is '" << pattern[0] << "', pf = " << pf[0] 
                << std::endl;

    for (int k = 0, i = 1; i < pattern.length(); ++i)
    //заполняем pf
    {
        std::cout << "Curr elem is '" << pattern[i] << "'.";

        while (k > 0 && pattern[i] != pattern[k])
        {
            k = pf[k-1];
            std::cout << " pattern[i] (" << pattern[i] << ") != pattern[k] ("
                        << pattern[k] << "), => k = pf[k-1].";
            if (k == 0)
                std::cout << " No identical items found => k = 0.";
        }

        if (pattern[i] == pattern[k])
        {
            std::cout << " pattern[i] (" << pattern[i] << ") == pattern[k] ("
                        << pattern[k] << "), => k++.";
            k++;
        }

        pf[i] = k;
        std::cout << " Pref. fun = " << pf[i] << std::endl;
    }
    std::cout << "Pattern - " << pattern << std::endl;
    std::cout << "Pref.fun- ";
    for (auto var : pf)
        std::cout << var;
    std::cout << std::endl;
}

vector<int> FullString::KMP_search()
{
    vector<int> res; // массив результата

    for (int k = 0, i = 0; i < S.length(); i++)
    {
        // Если предыдущий суффикс нельзя расширить, нужно попытаться взять суффикс меньшего размера
        while (k > 0 && pattern[k] != S[i])
            k = pf[k-1];

        // Если символы справа от префикса и суффикса совпадают, суффикс расширяется
        if (pattern[k] == S[i])
            k++;

        //Найдено вхождение
        if (k == pattern.length()) {
            res.push_back(i - k + 1);
        }

        cout << "Current symbol: " << S[i] << endl;
        cout << "Current match: ";
        for (int l=0; l < k; l++)
            cout << pattern[l];
        cout << endl;
    }

    if (res.empty()) {
        res.push_back(-1);
    }
    
    return res;
}

void FullString::isCyclShift()
{
    //Если длины не равны, сразу откат
    if (pattern.length() != S.length()) 
    {
        cout << -1 << endl;
        return;
    }

    for (int k = 0, i = 0; i < pattern.length()*2; i++)
    {
        //j - циклический обход строки
        int j = i % pattern.length();

        // Если предыдущий суффикс нельзя расширить, нужно попытаться взять суффикс меньшего размера
        while (k > 0 && S[k] != pattern[j])
            k = pf[k-1];

        // Если символы справа от префикса и суффикса совпадают, суффикс расширяется
        if (S[k] == pattern[j])
            k++;
        
        //Найдено вхождение
        if (k == pattern.length()) {
            cout << pattern[k-1];
            cout << endl;
			cout << i - k + 1 << endl;
			return;
		}
        cout << endl << "Current symbol: " << S[j] << endl;
        cout << "Current match: ";
        for (int l=0; l < k; l++)
            cout << pattern[l];
    }
    cout << -1 << endl;
}

int main()
{
    FullString str;
    cout << "***KMP Search***" << endl;
    std::vector<int> res = str.KMP_search();

    cout << res[0];
    for (int i = 1; i < res.size(); i++)
        cout << ", " << res[i];
    cout << endl << "***Cycle Shift Check***" << endl;
    str.isCyclShift();

    return 0;
}
