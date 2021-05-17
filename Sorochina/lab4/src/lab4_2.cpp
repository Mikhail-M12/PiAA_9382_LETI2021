#include <iostream>
#include <string>
#include <vector>

#define COMMENTS

void input(std::string &A, std::string &B);
//функция, считывающая ввод
std::vector<int> initialPi(std::string &p);
//функция вычисления префикс-функции
int answer(std::string &p, std::string &t);
//функция поиска ответа

int main()
{
    std::string A, B;
#ifdef COMMENTS
    std::cout << "Введите 2 строки\n";
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

std::vector<int> initialPi(std::string &p)
//функция вычисления префикс-функции
//возвращает вектор - префикс-функцию
{
    std::vector<int> pi;
    int len = p.length();
    pi.resize(len);             //меняем размер массива на длину шаблона
    pi[0] = 0;                  //первый элемент всегда 0
    int suffix = 1, prefix = 0; //suffix - индекс для суффикса, prefix - для префикса
    while (suffix < len)
    {
        if (p[suffix] == p[prefix])
        {
            pi[suffix] = prefix + 1;
            suffix++;
            prefix++;
        }
        else
        {
            if (prefix == 0)
            {
                pi[suffix] = 0;
                suffix++;
            }
            else
            {
                prefix = pi[prefix - 1];
            }
        }
    }
    return pi;
}

int answer(std::string &p, std::string &t)
{
    int ans = -1;
    std::vector<int> pi;
    int p_len = p.length();
    pi.resize(p_len);
    pi = initialPi(p); //считаем префикс-функцию для р aka А
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

    int t_ind = 0, p_ind = 0;
    bool flag = false; //флаг для отметки первый раз идем по строке или уже второй
    int t_len = t.length();
    while (t_ind < t_len)
    {
#ifdef COMMENTS
        std::cout << "\tСравним A[" << t_ind << "] '" << t[t_ind] << "' и B[" << p_ind << "] '" << p[p_ind] << "'\n";
#endif
        if (t[t_ind] == p[p_ind])
        {
#ifdef COMMENTS
            std::cout << "\t\tРавны\n";
#endif
            t_ind++;
            p_ind++;
            if (p_ind == p_len)
            {
                if (flag)
                {
                    t_ind += t_len;
                }
#ifdef COMMENTS
                std::cout << "\t\tНашли циклический сдвиг. Начинается с " << t_ind - p_len << "\n";
#endif
                ans = t_ind - p_len;
                return ans;
            }
        }
        else
        {
#ifdef COMMENTS
            std::cout << "\t\tНЕ равны\n";
#endif
            if (p_ind == 0)
            {
                t_ind++;
            }
            else
            {
                p_ind = pi[p_ind - 1];
            }
        }
        if (t_ind == t_len && flag == false) //если дошли до конца строки первый раз,
        {
            t_ind = 0;   //то переход на начало
            flag = true; //меняем флаг
        }
    }
#ifdef COMMENTS
    std::cout << "Дошли до конца текста. Конец поиска.\n";
#endif
    return ans;
}