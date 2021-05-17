#include <iostream>
#include <string>
#include <vector>

#define COMMENTS

void input(std::string &p, std::string &t);
//функция, считывающая ввод
std::vector<int> initialPi(std::string &p);
//функция вычисления префикс-функции
std::vector<int> answer(std::string &p, std::string &t);
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

std::vector<int> answer(std::string &p, std::string &t)
//функция поиска ответа
//возвращает вектор, элементы которого - индексы начал вхождения. Если он пуст, то вхождений нет
{
    std::vector<int> ans; //вектор для записи ответа
    std::vector<int> pi;
    int p_len = p.length();
    pi.resize(p_len);
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

    int t_ind = 0, p_ind = 0; //t_ind - индекс для прохода по тексту, p_ind - по шаблону
    int t_len = t.length();
    while (t_ind < t_len)
    {
#ifdef COMMENTS
        std::cout << "\tСравним t[" << t_ind << "] '" << t[t_ind] << "' и p[" << p_ind << "] '" << p[p_ind] << "'\n";
#endif
        if (t[t_ind] == p[p_ind])
        {
#ifdef COMMENTS
            std::cout << "\t  Равны\n";
#endif
            t_ind++;
            p_ind++;
            if (p_ind == p_len)
            {
#ifdef COMMENTS
                std::cout << "\t  Дошли до конца шаблона. Шаблон содержится в тексте с " << t_ind - p_len << "\n";
#endif
                ans.push_back(t_ind - p_len);
                p_ind = pi[p_ind - 1];
            }
        }
        else
        {
#ifdef COMMENTS
            std::cout << "\t  НЕ равны\n";
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
    }
#ifdef COMMENTS
    std::cout << "Дошли до конца текста. Конец поиска.\n";
#endif
    return ans;
}