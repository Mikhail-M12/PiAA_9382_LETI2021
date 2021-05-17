#include <iostream>
#include <string>
#include <vector>

#define COMMENTS

void input(std::string &p, std::string &t);
//функция, считывающая ввод
std::vector<int> initialPi(std::string p);
//функция вычисления префикс-функции
std::vector<int> answer(std::string p, std::string t);
//функция поиска ответа

int main()
{
    std::string p, t; //р - шаблон, t - текст
#ifdef COMMENTS
    std::cout << "Введите шаблон и текст\n";
#endif
    input(p, t);

    std::vector<int> ans = answer(p, t);
#ifdef COMMENTS
    std::cout << "Ответ:\n";
#endif
    if (ans.size() == 0)
    {
#ifdef COMMENTS
        std::cout << "Текст не содержит вхождений шаблона\n";
#endif
        std::cout << "-1\n";
        return 0;
    }
    for (int i = 0; i < ans.size() - 1; i++)
    {
        std::cout << ans[i] << ",";
    }
    std::cout << ans[ans.size() - 1] << '\n';
    return 0;
}

void input(std::string &p, std::string &t)
//функция, считывающая ввод
{
    getline(std::cin, p);
    getline(std::cin, t);
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

std::vector<int> answer(std::string p, std::string t)
//функция поиска ответа
//возвращает вектор, элементы которого - индексы начал вхождения. Если он пуст, то вхождений нет
{
    std::vector<int> ans; //вектор для записи ответа
    std::vector<int> pi;
    int len = p.length();
    pi.resize(len);
    pi = initialPi(p); //вычисляем префикс-функцию для шаблона
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

    int k = 0, l = 0; //k - индекс для прохода по тексту, l - по шаблону
    while (k < t.length())
    {
#ifdef COMMENTS
        std::cout << "\tСравним t[" << k << "] '" << t[k] << "' и p[" << l << "] '" << p[l] << "'\n";
#endif
        if (t[k] == p[l])
        {
#ifdef COMMENTS
            std::cout << "\t  Равны\n";
#endif
            k++;
            l++;
            if (l == len)
            {
#ifdef COMMENTS
                std::cout << "\t  Дошли до конца шаблона. Шаблон содержится в тексте с " << k - len << "\n";
#endif
                ans.push_back(k - len);
                l = pi[l - 1];
            }
        }
        else
        {
#ifdef COMMENTS
            std::cout << "\t  НЕ равны\n";
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
    }
#ifdef COMMENTS
    std::cout << "Дошли до конца текста. Конец поиска.\n";
#endif
    return ans;
}