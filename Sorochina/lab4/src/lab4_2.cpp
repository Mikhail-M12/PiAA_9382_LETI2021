#include <iostream>
#include <string>
#include <vector>

//#define COMMENTS

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
    if (A.length() != B.length()) //если длина строк не равна, то точно не циклический сдвиг
    {
#ifdef COMMENTS
        std::cout << "Строки разной длины\n";
#endif
        std::cout << "-1\n";
        return 0;
    }
    if (A == B) //если равны, то сдвиг равен 0
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

int answer(std::string &p, std::string &t)
{
    int ans = -1;
    std::vector<int> pi;
    int p_len = p.length();
    pi.resize(p_len);
    pi = initialPi(p); //считаем префикс-функцию для р aka А
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

    int t_ind = 0, p_ind = 0;
    bool flag = false; //флаг для отметки первый раз идем по строке или уже второй
    int t_len = t.length();
    while (t_ind < t_len) //пока индекс не равен длине строки
    {
#ifdef COMMENTS
        std::cout << "\n\tСравним A[" << t_ind << "] '" << t[t_ind] << "' и B[" << p_ind << "] '" << p[p_ind] << "'\n";
#endif
        if (t[t_ind] == p[p_ind]) //если символы равны, то
        {
#ifdef COMMENTS
            std::cout << "\t\tРавны\n";
            std::cout << "\t  Текущая совпадающая длина: " << p_ind << "\n";
#endif
            t_ind++; //увеличиваем индексы
            p_ind++;
            if (p_ind == p_len) //если конец шаблона, то
            {
                if (flag) //если это второй проход по строке
                {
                    t_ind += t_len; //добавляем к индексу длину строки
                }
#ifdef COMMENTS
                std::cout << "\t\tНашли циклический сдвиг. Начинается с " << t_ind - p_len << "\n";
#endif
                ans = t_ind - p_len;
                return ans;
            }
        }
        else //если символы не равны
        {
#ifdef COMMENTS
            std::cout << "\t\tНЕ равны\n";
#endif
            if (p_ind == 0) //есди первый элемент строки-шаблона
            {
#ifdef COMMENTS
                std::cout << "\t  Проверяли первый элемент строки-шаблона, значит идем дальше по тексту\n";
#endif
                t_ind++; //увеличиваем индекс в строке-тексте
            }
            else //если не первый эл-т
            {
#ifdef COMMENTS
                std::cout << "\t  Новый индекс строки-шаблона: " << pi[p_ind - 1] << "(" << p[pi[p_ind - 1]] << ")\n";
#endif
                p_ind = pi[p_ind - 1]; //меняем индекс строки-шаблона
            }
        }
        if (t_ind == t_len && flag == false) //если дошли до конца строки первый раз,
        {
#ifdef COMMENTS
            std::cout << "\t  Первый раз дошли до конца строки-текста. Меняем флаг и переходим на первый элемент.\n";
#endif
                t_ind = 0; //то переход на начало
            flag = true;   //меняем флаг
        }
    }
#ifdef COMMENTS
    std::cout << "Дошли до конца текста. Конец поиска.\n";
#endif
    return ans;
}