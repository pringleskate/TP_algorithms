/*
 *
 * Придиус Екатерина, АПО-13
 *
 * Задача 7_2
 *
 *
     Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 10^6.
     Отсортировать массив методом поразрядной сортировки LSD по байтам.

 */

#include <iostream>

// получаем n-ый байт числа
int getByte(unsigned long long number, size_t n) {
    return number >> (8 * n) & 0xFF;
}

// сортировка подсчетом массива чисел по n-ому байту
void countingSort(unsigned long long *data, size_t n, const size_t byte) {
    size_t counters[256];

    for(size_t i = 0; i < 256; i++)
        counters[i] = 0;

    // считаем количество чисел с разными значениями байта (от 0 до 255)
    for(size_t i = 0; i < n; i++)
        counters[getByte(data[i], byte)]++;

    // расчитываем первые индексы для вставки чисел
    for(size_t i = 1; i < 256; i++)
        counters[i] += counters[i - 1];


    // массив для результатов
    unsigned long long *tmp = new unsigned long long[n];

    // создаем отсортированный массив результатов
    for(size_t i = n - 1; ; i--) {
        tmp[--counters[getByte(data[i], byte)]] = data[i];
        if(i == 0)
            break;
    }

    // переписываем отсортированный массив в исходный
    for(size_t i = 0; i < n; i++)
        data[i] = tmp[i];

    delete [] tmp;
}


void LSDSort(unsigned long long *array, size_t n) {
    size_t totalBytes = sizeof(long long);

    for(size_t i_byte = 0; i_byte < totalBytes; i_byte++)
        countingSort(array, n, i_byte);
}

int main()
{
    size_t n = 0;
    std::cin >> n;

    unsigned long long *array = new unsigned long long[n];
    for(size_t i = 0; i < n; i++)
        std::cin >> array[i];

    LSDSort(array, n);

    for(size_t i = 0; i < n; i++)
        std::cout << array[i] << " ";

    delete [] array;

    return 0;
}