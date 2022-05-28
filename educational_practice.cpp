#include <iostream>
#include <string>
#include <iostream>
#include <Windows.h> 
#include <vector> 
#include <fstream>

using namespace std;

vector<string> splitString(string file_name, string& original_string);
int sortAndTime(vector<string>& words);
void sort(vector <string>& words);
int number(string str);
void writingToFileResult(string name_file, vector<string> words);
vector<int> numberWords(vector<string> words);
void writingToFileAnalysis(string name_file, string original_string, int time, int word_count, vector<int> number_words_array);

int main()
{
    setlocale(0, ""); // поддержка кириллицы в консоли (вывод)

    string name_file, original_string; //переменные имени файла и исходной строки
    cout << "Введите номер файла: ";
    cin >> name_file;

    //разбивка строки, полученной из текста, на массив слов
    vector <string> words = splitString(name_file, original_string);

    //сортировка и время 
    int time = sortAndTime(words);

    //запись в файл result
    writingToFileResult(name_file, words);

    //подсчет количества слов на каждую букву
    vector<int> number_words_array = numberWords(words);

    //запись в файл analysis 
    writingToFileAnalysis(name_file, original_string, time, words.size(), number_words_array);

    return 0;
}

vector<string> splitString(string name_file, string& original_string)
{
    fstream file_original; //создаем переменную файла

    file_original.open("original_" + name_file + ".txt", ios::in); // открываем файл на запись из файла

    vector<string> words;
    string s = "";

    char ch;

    if (file_original.is_open()) //если файл открылся 
    {
        while (file_original.get(ch)) //пока можем считать символ из файла
        {
            original_string += ch;; //прибавляем символ к строке 
        }
        file_original.close(); // Закрытие файла
    }
    else
    {
        cout << "Исходный файл не открылся";
        exit(0);
    }

    original_string = '"' + original_string + '"'; //добавляем кавычки, чтобы потом не было проблем со считыаванием последнего символа

    char symbol, symbol_next;

    for (int i = 0; i < original_string.length() - 1; i++)
    {
        symbol = original_string[i];
        symbol_next = original_string[i + 1];

        if ((symbol >= 'А' && symbol <= 'Я' || symbol >= 'а' && symbol <= 'я' || symbol == 'Ё' || symbol == 'ё') //если символ строки буква из кириллицы 
            || //или 
            (symbol == '-' && (symbol_next >= 'А' && symbol_next <= 'Я' || symbol_next >= 'а' && symbol_next <= 'я' || symbol_next == 'Ё' || symbol_next == 'ё') && s.size() != 0)) //символ это -, а дальше идет буква из кириллицы и размер строки не равен 0, то есть есть часть слова перед тире

        {
            s += symbol; //собираем слово 
            if ((((symbol >= 'А' && symbol <= 'Я') || symbol == 'Ё') && //если символ буквы большого регистра
                ((symbol_next < 'А' || symbol_next > 'Я' && symbol_next < 'а' || symbol_next > 'я') && symbol != 'Ё' && symbol != 'ё')) //а следующий символ - все кроме букв обоих регистров
                || //или 
                (((symbol >= 'а' && symbol <= 'я') || symbol == 'ё') && //символ - буква маленького регистра
                    (symbol_next < 'а' || symbol_next > 'я') && symbol_next != 'ё' && symbol_next != '-') //а следующий символ - все кроме букв маленького регистра
                )
            {
                words.push_back(s); //добавляем в массив строку
                s = ""; //обнуляем строку
            }
        }
    }
    return words;
}

int sortAndTime(vector<string>& words)
{
    string tmp;

    int start_time = clock();
    sort(words);
    int end_time = clock();

    return end_time - start_time;
}

void sort(vector <string>& words)
{

    string temp; // временная переменная для хранения значения элемента сортируемого массива
    int last_index; // индекс предыдущего элемента

    for (int i = 1; i < words.size(); i++)
    {
        temp = words[i]; // инициализируем временную переменную текущим значением элемента массива
        last_index = i - 1; // запоминаем индекс предыдущего элемента массива
        while (last_index >= 0 && number(words[last_index]) > number(temp)) // пока индекс не равен 0 и предыдущий элемент массива больше текущего
        {
            words[last_index + 1] = words[last_index]; // перестановка элементов массива
            words[last_index] = temp;
            last_index--;
        }
    }
}

int number(string str)
{
    string cyrillic_low_reg = "абвгдеёжзийклмнопрстуфхцчшщъэьэюя"; //алфавит русский

    if (str[0] >= 'а' && str[0] <= 'я' || str[0] == 'ё') //если нижнего регистра то возвращем номер в русском алфавите
    {
        return cyrillic_low_reg.find(str[0]);
    }
    else //иначе
    {
        if (str[0] == 'Ё') //если Ё то ищем где маленькая Ё находится
        {
            char ch = 'ё';
            return cyrillic_low_reg.find(ch);
        }
        char ch = str[0] + 'а' - 'А'; //если не Ё то ищем где находится маленькая буква за счет нахождения кода маленькой буквы, за счет прибавления к большой букве разности между маленькой и большой буквой
        return cyrillic_low_reg.find(ch);
    }
}

void writingToFileResult(string name_file, vector <string> words)
{
    fstream file_result;
    file_result.open("result_" + name_file + ".txt", ios::out); // открываем файл на запись в него, если файла нет, то он создастся

    //вывод слов
    for (int i = 0; i < words.size(); i++)
    {
        file_result << words[i] << endl;
    }
    file_result.close();
}

vector<int> numberWords(vector<string> words)
{
    vector<int> number_words_array(33); //массив длиной 33, изначально заполнен нулями. 
    //Каждый элемент - это количество слов на букву, номер в алфавите которой, равен индексу элемента в массиве

    string cyrillic_high_reg = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; //алфавит русский
    string cyrillic_low_reg = "абвгдеёжзийклмнопрстуфхцчшщъэьэюя"; //алфавит русский
    for (int i = 0; i < words.size(); i++)
    {
        for (int j = 0; j < cyrillic_high_reg.size(); j++) //проходим по всему массиву количества
        {
            if (cyrillic_high_reg[j] == words[i][0]) //если первая буква равна какой-то букве из верхнего регистра, то увеличиваем
                number_words_array[j]++;
            if (cyrillic_low_reg[j] == words[i][0]) //если первая буква равна какой-то букве из нижнего регистра, то увеличиваем
                number_words_array[j]++;
        }
    }
    return number_words_array;
}

void writingToFileAnalysis(string name_file, string original_string, int time, int word_count, vector<int> number_words_array)
{
    fstream file_analysis;

    file_analysis.open("analysis_" + name_file + ".txt", ios::out); // открываем файл на запись в него, если файла нет, то он создастся

    file_analysis
        << "Введенный текст: " << endl
        << original_string << endl
        << "Вариант 15: кириллица, по алфавиту, по возрастанию, игнорировать числа, сортировка вставками " << endl
        << "Количество слов: " << word_count << endl
        << "Время сортировки: " << static_cast<double>(time) / 1000 << " сек" << endl //static_cast это приведение типа
        << "Статистика: " << endl;

    string cyrillic_low_reg = "абвгдеёжзийклмнопрстуфхцчшщъэьэюя"; //алфавит русский

    //вывод количества слов на каждую букву
    for (int i = 0; i < number_words_array.size(); i++)
    {
        file_analysis << cyrillic_low_reg[i] << ": " << number_words_array[i] << endl; //выводим букву и количество слов на эту букву
    }
    file_analysis.close(); //закрываем файл
}
