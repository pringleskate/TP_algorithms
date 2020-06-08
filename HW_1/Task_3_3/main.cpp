/*
    Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.
    Требования: Очередь должна быть реализована в виде класса. Стек тоже должен быть реализован в виде класса.

    Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
    Формат входных данных.
    В первой строке количество команд n. n ≤ 1000000.
    Каждая команда задаётся как 2 целых числа: a b.
    a = 2 - pop front
    a = 3 - push back
    Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
    Для очереди используются команды 2 и 3.
    Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
    Формат выходных данных.
    Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

*/

#include <iostream>

#define MIN_SIZE 3
#define MEM_INCREASE 2

template <typename T>
class Stack {

public:
    Stack();

    ~Stack();

    void push(const T &element);

    T pop();

    bool isEmpty();

    void print_stack();

private:
     T *buffer;

     size_t capacity;

     size_t size;

     int head;

     void resize();
};

template <typename T>
class Queue {
public:
    Queue();

    ~Queue();

    void enqueue(const T &element);

    T dequeue();

    void print_queue();

private:
    Stack<T> leftStack, rightStack;
};


//////Queue methods implemetation
template <typename T>
Queue<T>::Queue() = default; /*{
   *//* leftStack();
    rightStack();*//*
}*/

template <typename T>
Queue<T>::~Queue() = default; /*{
    *//*~leftStack();
    ~rightStack();*//*
}*/

template <typename T>
void Queue<T>::enqueue(const T &element) {
    leftStack.push(element);
}


template <typename T>
T Queue<T>::dequeue() {
    if (!rightStack.isEmpty())
        return rightStack.pop();
    else {
        while (!leftStack.isEmpty())
            rightStack.push(leftStack.pop());
        return rightStack.pop();
    }
}

template <typename T>
void Queue<T>::print_queue() {
    std::cout << "leftStack: " << std::endl;
    leftStack.print_stack();

    std::cout << "rightStack: " << std::endl;
    rightStack.print_stack();
}

/////Stack methods implementation
template <typename T>
Stack<T>::Stack() {
    size = 0;
    head = 0;
    capacity = MIN_SIZE;
    buffer = new T[MIN_SIZE];
}


template <typename T>
Stack<T>::~Stack() {
    delete [] buffer;
}


template <typename T>
void Stack<T>::push(const T &element) {
    if (head == capacity - 1)
        resize();

    head++;
    buffer[head] = element;
    size++;
}

template <typename T>
T Stack<T>::pop() {
    if (isEmpty())
        return -1;

    T ret_val = buffer[head];
    head--;
    size--;
    return ret_val;
}

template <typename T>
bool Stack<T>::isEmpty() {
    return !size;
}


template <typename T>
void Stack<T>::resize() {
    size_t newCapacity = capacity * MEM_INCREASE;
    T *tmp = new T[newCapacity];

    std::copy(buffer, buffer + head + 1, tmp);

    capacity = newCapacity;
    delete [] buffer;
    buffer = tmp;
}

template <typename T>
void Stack<T>::print_stack() {
    for (int i = 0; i < capacity; i++) {
        if (i <= size)
            std::cout << " |" << buffer[i];
        else
            std::cout << " |";
    }

    std::cout << std::endl;
}



int main() {

    /*for testing stack
     *
     * Stack<int> stack;

    int n = 0;
    int cmd = 0;
    int element = 0;
    int ret_pop = 0;

    bool flag = true;

    std::cin >> n;
    if (!n)
        return 0;

    for (size_t i = 0; i < n; i++) {
        std::cin >> cmd >> element;

        switch (cmd) {
            case 1:
                stack.push(element);
                break;
            case 2:
                ret_pop = stack.pop();
                std::cout << "ret pop " << ret_pop << std::endl;
                if (element != ret_pop)
                    flag = false;
                break;
            default:
                break;
        }
        stack.print_stack();
    }

    if (flag)
        std::cout << "YES";
    else
        std::cout << "NO";
    return 0;

    a = 2 - pop front
    a = 3 - push back

     */


    Queue<int> queue;

    int n = 0;
    int cmd = 0;
    int element = 0;
    int ret_pop = 0;

    bool flag = true;

    std::cin >> n;
    if (!n)
        return 0;

    for (size_t i = 0; i < n; i++) {
        std::cin >> cmd >> element;

        switch (cmd) {
            case 3:
                queue.enqueue(element);
                break;
            case 2:
                ret_pop = queue.dequeue();
                //std::cout << "ret pop " << ret_pop << std::endl;
                if (element != ret_pop)
                    flag = false;
                break;
            default:
                break;
        }
       queue.print_queue();
    }

    if (flag)
        std::cout << "YES";
    else
        std::cout << "NO";
    return 0;
}
