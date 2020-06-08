/*
 *
 * Придиус Екатерина, АПО-13
 *
 * Задача 1_1
 *
 *
    Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
    Хранимые строки непустые и состоят из строчных латинских букв.
    Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
    Начальный размер таблицы должен быть равным 8-ми.
    Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
    Структура данных должна поддерживать операции добавления строки в множество,
    удаления строки из множества и проверки принадлежности данной строки множеству.

    Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
    g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.


stdin               stdout
+ hello             OK
+ bye               OK
? bye               OK
+ bye               FAIL
- bye               OK
? bye               FAIL
? hello             OK


*/


#include <iostream>
#include <vector>

#define DEFAULT_SIZE 8
#define MAX_ALPHA 0.75
#define BUF_INCREASE 2

class StringHasher {
public:
    explicit StringHasher(size_t prime = 71): prime(prime) {}
    size_t operator()(const std::string &str) const {
        size_t hash = 0;
        for (char i : str)
            hash = hash * prime + i;

        return hash;
    }
private:
    size_t prime;
};


template <typename Hasher>
class HashTable {
private:
    size_t size;
    std::vector<std::string> table;
    Hasher hasher;

    void grow() {
        size_t new_size = table.size() * BUF_INCREASE;
        std::vector<std::string> new_table;

        new_table.reserve(new_size);
        for (int i = 0; i < new_size; i++)
            new_table.push_back("NIL");


        for (int i = 0; i < table.size(); i++) {
            if (table[i] != "NIL") {
                size_t hash = hasher(table[i]) % new_table.size();

                for (int j = 0; j < table.size(); j++) {
                    if (new_table[hash] == "NIL") {
                        new_table[hash] = table[i];
                        break;
                    }
                    else
                        hash = (hash + j + 1) % new_size;
                }
            }
        }
        table = new_table;
    }


public:
    explicit HashTable(size_t initial_size = DEFAULT_SIZE) : size(0), table(initial_size) {
        for (auto & i : table)
            i = "NIL";
  
    ~HashTable() = default;

    bool Add(const std::string &key) {
        if (size >= table.size() * MAX_ALPHA)
            grow();

        int first_del = -1;
        size_t hash = hasher(key) % table.size();

        for (int i = 0; i < table.size(); i++) {
            if (table[hash] == key)
                return false;
            else if ((table[hash] == "DEL") && (first_del == -1))
                first_del = hash;
            else if(table[hash] == "NIL") {
                table[hash] = key;
                size++;
                return true;
            }

            hash = (hash + i + 1) % table.size();
        }

        table[first_del] = key;
        size++;
        return true;
    }

    bool Delete(const std::string &key) {
        size_t hash = hasher(key) % table.size();

        for (int i = 0; i < table.size(); i++) {
            if (table[hash] == key) {
                table[hash] = "DEL";
                size--;
                return true;
            }
            else if (table[hash] == "NIL")
                return false;
            hash = (hash + i + 1) % table.size();
        }
        return false;
    }

    bool Has(const std::string &key) {
        size_t hash = hasher(key) % table.size();

        for (int i = 0; i < table.size(); i++) {
            if (table[hash] == key) {
                return true;
            }
            else if(table[hash] == "NIL")
                return false;
            hash = (hash + i + 1) % table.size();
        }
        return false;
    }
};

int main() {
    HashTable<StringHasher> table;

    char op;
    std::string str;

    while (std::cin >> op >> str)
    {
        switch (op)
        {
            case '?':
            {
                std::cout << (table.Has(str) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '+':
            {
                std::cout << (table.Add(str) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '-':
            {
                std::cout << (table.Delete(str) ? "OK" : "FAIL") << std::endl;
                break;
            }
        }
    }

    return 0;
}

