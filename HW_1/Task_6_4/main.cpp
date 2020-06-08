/*
 *
 * Придиус Екатерина, АПО-13
 *
 * Задача 6_4
 *
 *
     Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..10^9] размера n.

     Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции
     с индексом k ∈[0..n-1] в отсортированном массиве.
     Напишите нерекурсивный алгоритм.
     Требования к дополнительной памяти: O(n).
     Требуемое среднее время работы: O(n).

     Функцию Partition следует реализовывать методом прохода двумя итераторами в направлении от конца массива к началу.
     Реализуйте стратегию выбора опорного элемента “случайный элемент”.

*/

#include <iostream>
#include <cstdlib>

int partition(int *array, int left, int right);

int kStat(int *array, int k, int left, int right);

int main() {
    int n = 0, k = 0;
    std::cin >> n >> k;

    int *array = new int[n];

    for (int i = 0; i < n; ++i)
        std::cin >> array[i];

    std::cout << kStat(array, k, 0, n - 1);

    delete [] array;

    return 0;
}


int partition(int *array, int left, int right) {
    if (left != right)
        std::swap(array[left + random() % (right - left)], array[left]);

    int x = array[left];

    // два счетчика справа, двигаются в одном направлении
    int i = right + 1;

    for (int j = right; j >= left; --j)
        if (array[j] >= x)
            std::swap(array[--i], array[j]);

    return i;
}

int kStat(int *array, const int k, int left, int right) {
    while(true)
    {
        int pivotPos = partition(array, left, right);

        if (pivotPos < k)
            left = pivotPos + 1;

        else if (pivotPos > k)
            right = pivotPos - 1;

        else
            return array[k];
    }
}
