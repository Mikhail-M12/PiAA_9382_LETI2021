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
    if (ans.size() == 0) //если вектор пуст, значит нет вхождений шаблона в текст
    {
#ifdef COMMENTS
        std::cout << "Текст не содержит вхождений шаблона\n";
#endif
        std::cout << "-1\n";
        return 0;
    }
    for (int i = 0; i < ans.size() - 1; i++) //вывод ответа через заптую
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
#ifdef COMMENTS
        std::cout << "\n\t\tИндекс суффикса: " << suffix << "(" << p[suffix] << ")"
                  << ", индекс префикса: " << prefix << "(" << p[prefix] << ")"
                  << "\n";
#endif
        if (p[suffix] == p[prefix]) //если символы равны,
        {
#ifdef COMMENTS
            std::cout << "\t\tСимволы равны. Значение для " << suffix << "-го элемента равно " << prefix + 1 << "\n";
#endif
            pi[suffix] = prefix + 1; //то записываем значение префикс-функции для эл-та
            suffix++;                //увеличиваем индексы
            prefix++;
        }
        else // Если символы не равны
        {
#ifdef COMMENTS
            std::cout << "\t\tСимволы не равны.";
#endif
            if (prefix == 0) // ...и индекс префикса равен нулю
            {
#ifdef COMMENTS
                std::cout << " Значение для " << suffix << "-го элемента равно 0\n";
#endif
                pi[suffix] = 0; //то значение функции для данного эл-та равно 0
                suffix++;       //идем к след. эл-ту
            }
            else //...и индекс префикса не равен нулю
            {
#ifdef COMMENTS
                std::cout << "\n\t\tНовый индекс префикса: " << pi[prefix - 1] << "(" << p[pi[prefix - 1]] << ")\n";
#endif
                prefix = pi[prefix - 1]; //то меняем индекс особым образом
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
    std::cout << "\n\tПрефикс-функция шаблона:\n\t";
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
    while (t_ind < t_len) //пока не дойдем до конца текста
    {
#ifdef COMMENTS
        std::cout << "\n\tСравним t[" << t_ind << "] '" << t[t_ind] << "' и p[" << p_ind << "] '" << p[p_ind] << "'\n";
#endif
        if (t[t_ind] == p[p_ind]) //если знаки равны, то
        {
#ifdef COMMENTS
            std::cout << "\t  Равны\n";
            std::cout << "\t  Текущая совпадающая длина: " << p_ind << "\n";
#endif
            t_ind++; //увеличиваем оба индекса
            p_ind++;
            if (p_ind == p_len) //если прошли весь шаблон, то
            {
#ifdef COMMENTS
                std::cout << "\t  Дошли до конца шаблона. Шаблон содержится в тексте с " << t_ind - p_len << "\n";
#endif
                ans.push_back(t_ind - p_len); //записываем индекс начала вхождения
                p_ind = pi[p_ind - 1];        //и меняем индекс шаблона
            }
        }
        else //если знаки не равны,то
        {
#ifdef COMMENTS
            std::cout << "\t  НЕ равны\n";
#endif
            if (p_ind == 0) //если это первый элемент шаблона, то
            {
#ifdef COMMENTS
                std::cout << "\t  Проверяли первый элемент шаблона, значит идем дальше по тексту\n";
#endif
                t_ind++; //идем дальше по тексту
            }
            else //если не первый эл-т шаблона, то
            {
#ifdef COMMENTS
                std::cout << "\t  Новый индекс шаблона: " << pi[p_ind - 1] << "(" << p[pi[p_ind - 1]] << ")\n";
#endif
                p_ind = pi[p_ind - 1]; //меняем индекс шаблона
            }
        }
    }
#ifdef COMMENTS
    std::cout << "Дошли до конца текста. Конец поиска.\n";
#endif
    return ans;
}