/*
 *
 * Придиус Екатерина, АПО-13
 *
 * Задача 4_2
 *
 *
     Для сложения чисел используется старый компьютер. Время, затрачиваемое
     на нахождение суммы двух чисел равно их сумме. Таким образом для нахождения
     суммы чисел 1,2,3 может потребоваться разное время, в зависимости от порядка
     вычислений.
     ((1+2)+3) -> 1+2 + 3+3 = 9
     ((1+3)+2) -> 1+3 + 4+2 = 10
     ((2+3)+1) -> 2+3 + 5+1 = 11
     Требуется написать программу, которая определяет минимальное время, достаточное
     для вычисления суммы заданного набора чисел.

     Требуемое время работы O(n*log(n)).


    in                  out
    5
    5 2 3 4 6           45

    5
    3 7 6 1 9           56

*/

#include <iostream>
#include <cassert>

// Куча, реализованная с помощью массива (MinHeap)
template <typename T, typename Comparator>
class Heap {
private:
    T *buffer;                  //буфер для хранения элементов кучи
    size_t buffer_capacity;     //вместимость буфера
    size_t buffer_size;         //реальный размер буфера

public:
    explicit Heap(size_t size);
    ~Heap();

    int extractRoot();
    void siftUp(size_t index);
    void siftDown(size_t index);
    void insert(T element);

    int getBufferSize();
};

template <typename T, typename Comparator>
int getMinTime(Heap<T, Comparator> *heap);


int main() {
    size_t elements_count = 0;

    std::cin >> elements_count;
    assert(elements_count > 0);
    //считываем количество вводимых элементов, создаем кучу соответствующей вместимости
    Heap<int, std::less<>> heap(elements_count);

    int number = 0;

    for(size_t i = 0; i < elements_count; ++i) {
        //считываем элементы кучи и вставляем их в кучу
        std::cin >> number;
        heap.insert(number);
    }

    //выполняем поиск минимального времени сложения (сумма элементов кучи)
    std::cout << getMinTime(&heap);

    return 0;
}

template <typename T, typename Comparator>
Heap<T, Comparator>::Heap(size_t size): buffer_capacity(size), buffer_size(0) {
    buffer = new int[buffer_capacity];
}

template <typename T, typename Comparator>
Heap<T, Comparator>::~Heap() {
    delete [] buffer;
}

//извлекаем корневой элемент
template <typename T, typename Comparator>
int Heap<T, Comparator>::extractRoot() {
    int tmp = buffer[0];
     
    //на место корневого элемента ставим последний элемент в куче и сортируем ее в правильном порядке 
    buffer[0] = buffer[buffer_size -  1];
    std::swap(buffer[0], buffer[--buffer_size]);
    siftDown(0);

    return tmp;
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::siftDown(size_t index) {
    Comparator cmp = Comparator();

    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    // Поскольку в нашем случае используется min heap, мы ищем меньшего сына, если такой есть.
    int least = index;

    if(left < buffer_size && cmp(buffer[left],buffer[index]))
        least = left;

    if(right < buffer_size && cmp(buffer[right], buffer[least]))
        least = right;

    // Если меньший сын есть, то проталкиваем корень в него.
    if(least != index) {
        std::swap(buffer[index], buffer[least]);
        siftDown(least);
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::siftUp(size_t index) {
    size_t parent;
    Comparator cmp = Comparator();

    while(index > 0) {
        parent = (index - 1) / 2;

        if(cmp(buffer[index], buffer[parent]))
        //if(buffer[index] <= buffer[parent])
            std::swap(buffer[index], buffer[parent]);

        index = parent;
    }
}

//добавление элемента в кучу
template <typename T, typename Comparator>
void Heap<T, Comparator>::insert(T element) {
    buffer[buffer_size] = element;
    siftUp(buffer_size);
    buffer_size++;
}

template <typename T, typename Comparator>
int Heap<T, Comparator>::getBufferSize() {
    return buffer_size;
}

//Расчет минимального времени для подсчета суммы
template <typename T, typename Comparator>
int getMinTime(Heap<T, Comparator> *heap)
{
    int time = 0; //затраченное время
    int sum = 0; //промежуточная сумма

    while (heap->getBufferSize() > 1) {
        sum = heap->extractRoot(); //достаем первый элемент
        sum += heap->extractRoot(); //достаем второй элемент и суммируем оба
        heap->insert(sum); //полученную сумму добавляем в кучу и сортируем ее
        time += sum; //затраченное на подсчет время увеличивается на полученную сумму 
    }

    return time;
}
