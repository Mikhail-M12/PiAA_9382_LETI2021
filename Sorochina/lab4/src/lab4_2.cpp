#include <iostream>
#include <string>
#include <vector>

#define COMMENTS

void input(std::string &A, std::string &B);
//функция, считывающая ввод
std::vector<int> initialPi(std::string p);
//функция вычисления префикс-функции
int answer(std::string p, std::string t);
//функция поиска ответа

int main()
{
    std::string A, B;
#ifdef COMMENTS
    std::cout << "Введите шаблон и текст\n";
#endif
    input(A, B);
    if (A.length() != B.length())
    {
#ifdef COMMENTS
        std::cout << "Строки разной длины\n";
#endif
        std::cout << "-1\n";
        return 0;
    }
    if (A == B)
    {
#ifdef COMMENTS
        std::cout << "Строки совпадают\n";
#endif
        std::cout << "0\n";
        return 0;
    }
    int ans = answer(B, A);
#ifdef COMMENTS
    std::cout << "Ответ:\n";
    if (ans == -1)
    {
        std::cout << "Не является сдвигом\n";
    }
#endif
    std::cout << ans << "\n";
    return 0;
}

void input(std::string &A, std::string &B)
//функция, считывающая ввод
{
    getline(std::cin, A);
    getline(std::cin, B);
}

std::vector<int> initialPi(std::string p)
//функция вычисления префикс-функции
//возвращает вектор - префикс-функцию
{
    std::vector<int> pi;
    int len = p.length();
    pi.resize(len);   //меняем размер массива на длину шаблона
    pi[0] = 0;        //первый элемент всегда 0
    int i = 1, j = 0; //i - индекс для суффикса, j - для префикса
    while (i < len)
    {
        if (p[i] == p[j])
        {
            pi[i] = j + 1;
            i++;
            j++;
        }
        else
        {
            if (j == 0)
            {
                pi[i] = 0;
                i++;
            }
            else
            {
                j = pi[j - 1];
            }
        }
    }
    return pi;
}

int answer(std::string p, std::string t)
{
    int ans = -1;

    std::vector<int> pi;
    int len = p.length();
    pi.resize(len);
    pi = initialPi(p);
#ifdef COMMENTS
    std::cout << "\tПрефикс-функция шаблона:\n\t";
    for (auto i : p)
    {
        std::cout << i << " ";
    }
    std::cout << "\n\t";
    for (auto i : pi)
    {
        std::cout << i << " ";
    }
    std::cout << '\n';
#endif

    int k = 0, l = 0;
    bool flag = false;
    while (k < t.length())
    {
#ifdef COMMENTS
        std::cout << "\tСравним A[" << k << "] '" << t[k] << "' и B[" << l << "] '" << p[l] << "'\n";
#endif
        if (t[k] == p[l])
        {
#ifdef COMMENTS
            std::cout << "\t\tРавны\n";
#endif
            k++;
            l++;
            if (l == len)
            {
                if (flag)
                {
                    k += t.length();
                }
#ifdef COMMENTS
                std::cout << "\t\tНашли циклический сдвиг. Начинается с " << k - len << "\n";
#endif
                ans = k - len;
                return ans;
            }
        }
        else
        {
#ifdef COMMENTS
            std::cout << "\t\tНЕ равны\n";
#endif
            if (l == 0)
            {
                k++;
            }
            else
            {
                l = pi[l - 1];
            }
        }
        if (k == t.length() && flag == false)
        {
            k = 0;
            flag = true;
        }
    }
#ifdef COMMENTS
    std::cout << "Дошли до конца текста. Конец поиска.\n";
#endif
    return ans;
}