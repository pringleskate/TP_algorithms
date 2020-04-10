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
class Heap {
private:
    int *buffer;         //буфер для хранения элементов кучи
    int buffer_capacity; //вместимость буфера
    int buffer_size;     //реальный размер буфера

public:
    explicit Heap(int size);
    ~Heap();

    int extractRoot();
    void siftUp(int index);
    void siftDown(int index);
    void insert(int element);

    int getBufferSize();
};

int getMinTime(Heap *heap);


int main() {
    int elements_count = 0;

    std::cin >> elements_count;
    assert(elements_count > 0);
    //считываем количество вводимых элементов, создаем кучу соответствующей вместимости
    Heap heap(elements_count);

    int number = 0;

    for(int i = 0; i < elements_count; ++i) {
        //считываем элементы кучи и вставляем их в кучу
        std::cin >> number;
        heap.insert(number);
    }

    //выполняем поиск минимального времени сложения (сумма элементов кучи)
    std::cout << getMinTime(&heap);

    return 0;
}


Heap::Heap(int size): buffer_capacity(size), buffer_size(0) {
    buffer = new int[buffer_capacity];
}

Heap::~Heap() {
    delete [] buffer;
}

//извлекаем корневой элемент
int Heap::extractRoot() {
    int tmp = buffer[0];
     
    //на место корневого элемента ставим последний элемент в куче и сортируем ее в правильном порядке 
    buffer[0] = buffer[buffer_size -  1];
    std::swap(buffer[0], buffer[--buffer_size]);
    siftDown(0);

    return tmp;
}

void Heap::siftDown(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    // Поскольку в нашем случае используется min heap, мы ищем меньшего сына, если такой есть.
    int least = index;

    if(left < buffer_size && buffer[left] < buffer[index])
        least = left;

    if(right < buffer_size && buffer[right] < buffer[least])
        least = right;

    // Если меньший сын есть, то проталкиваем корень в него.
    if(least != index) {
        std::swap(buffer[index], buffer[least]);
        siftDown(least);
    }
}

void Heap::siftUp(int index) {
    int parent;

    while(index > 0) {
        parent = (index - 1) / 2;

        if(buffer[index] <= buffer[parent])
            std::swap(buffer[index], buffer[parent]);

        index = parent;
    }
}

//добавление элемента в кучу
void Heap::insert(int element) {
    buffer[buffer_size] = element;
    siftUp(buffer_size);
    buffer_size++;
}

int Heap::getBufferSize() {
    return buffer_size;
}

//Расчет минимального времени для подсчета суммы 
int getMinTime(Heap *heap)
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
