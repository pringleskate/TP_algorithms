/*
 *
 * Придиус Екатерина, АПО-13
 *
 * Задача 3_2
 *
 * Реализовать дек с динамическим зацикленным буфером.
 *
    Формат входных данных.
    В первой строке количество команд n. n ≤ 1000000.
    Каждая команда задаётся как 2 целых числа: a b.
    a = 1 - push front
            a = 2 - pop front
            a = 3 - push back
            a = 4 - pop back
            Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
    Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
    Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
    Формат выходных данных.
    Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

    Требования: Дек должен быть реализован в виде класса.
 */



#include <iostream>
#include <cassert>

#define MIN_SIZE 10
#define MEM_INCREASE 2

template <typename T>
class Deque {
public:
    Deque();
    ~Deque();

    void push_front(const T &element);

    void push_back(const T &element);

    T pop_front();

    T pop_back();

    bool isEmpty();

    void print_deque() const;
private:
    int tail; //хвостовой элемент
    int head; //головной элемент

    T *buffer; //буфер для хранения элементов
    size_t capacity; //вместимость буфера
    size_t size; //количество реально хранимых элементов


    void resize();
};

//конструктор для дека
template <typename T>
Deque<T>::Deque(): head{0}, tail{0} {
    buffer = new T[MIN_SIZE]; //выделяем память для буфера
    capacity = MIN_SIZE; 
    size = 0;
}

template <typename T>
Deque<T>::~Deque() {
    delete [] buffer;
}

//проверка дека на пустоту
template <typename T>
bool Deque<T>::isEmpty() {
    return !size;
}

//вставка элемента в tail
template <typename T>
void Deque<T>::push_back(const T &element) {
    if (size + 1 > capacity)
        resize();

    buffer[tail] = element;
    size++;
    tail = (tail + 1) % capacity;
}

//вставка элемента в head
template <typename T>
void Deque<T>::push_front(const T &element) {
    if (size + 1 > capacity)
        resize();

    head = (head - 1 + capacity) % capacity;
    buffer[head] = element;
    size++;
}

//извлечение элемента из head
template <typename T>
T Deque<T>::pop_front() {
    if (isEmpty())
        return -1;

    T return_element = buffer[head];
    head = (head + 1) % capacity;
    size--;
    return return_element;
}

//извлечение элмента из tail
template <typename T>
T Deque<T>::pop_back() {
    if (isEmpty())
        return -1;

    tail = (tail - 1 + capacity) % capacity;
    size--;
    return buffer[tail];
}

//изменяем размер буфера при недостатке памяти для добавления нового элемента
template <typename T>
void Deque<T>::resize() {
    size_t newCapacity = capacity * MEM_INCREASE;
    T *tmp = new T[newCapacity]; //выделяем память

    //копируем содержимое дека
    if (head < tail) {
        std::copy(buffer + head, buffer + tail, tmp + head);
    }
    else {
        // копируем кусками зацикленный буфер, в котором tail опередил head 
        std::copy(buffer + head, buffer + capacity, tmp + head);
        std::copy(buffer, buffer + tail, tmp + capacity);
        tail = capacity + tail;
    }

    capacity = newCapacity;
    delete [] buffer;
    buffer = tmp;
}

//отладочная функция красивого рисования дека :)
template <typename T>
void Deque<T>::print_deque() const {
    std::cout << "head: " << head << "; tail: " << tail << std::endl;
    for (size_t i = 0; i < capacity; ++i) {
        if (i >= tail || i < head) {
            std::cout << "|x ";
        }
        else {
            std::cout << "|" << buffer[i];
        }

        if (i == head) {
            std::cout << "|>";
        }
        else if (i == tail) {
            std::cout << "<|";
        }
        else {
            std::cout << "|";
        }
    }
    std::cout << std::endl;
}

int main() {

    Deque<int> deq;

    int n = 0;
    int cmd = 0;
    int element = 0;
    int ret_pop = 0;

    bool flag = true;

    std::cin >> n;
    assert(n >= 0);

    for (size_t i = 0; i < n; i++) {
        std::cin >> cmd >> element;
        //считываем пользовательскую команду и вызываем соответствующий метод
        
        switch (cmd) {
            case 1:
                deq.push_front(element);
                break;
            case 2:
                ret_pop = deq.pop_front();
                if (element != ret_pop)
                    flag = false; //извлеченный элемент не соответствует введенному 
                break;
            case 3:
                deq.push_back(element);
                break;
            case 4:
                ret_pop = deq.pop_back();
                if (element != ret_pop)
                    flag = false; //извлеченный элемент не соответствует введенному 
                break;
            default:
                break;
        }
    }

    if (flag)
        std::cout << "YES";
    else
        std::cout << "NO"; //выводим, если хотя бы одно ожидаемое значение не не оправдалось

    return 0;
}
