/*Дан массив неотрицательных целых 64-разрядных чисел.
 * Количество чисел не больше 10^6.
 * Отсортировать массив методом MSD по битам (бинарный QuickSort).
In
3
4 1000000 7

Out
4 7 1000000
*/

#include <iostream>
#define BITS_IN_WORD 63

bool is_unit(unsigned long long number, int index);

void binary_quick_sort(unsigned long long *array, int first, int last, int binary_word);

int main() {
    int array_size;
    std::cin >> array_size;

    if (!array_size)
        return 1;

    unsigned long long *array = new unsigned long long [array_size];
    for (int i = 0; i < array_size; ++i)
        std::cin >> array[i];

    binary_quick_sort(array, 0, array_size - 1, BITS_IN_WORD);


    for (int i = 0; i < array_size; ++i)
        std::cout << array[i] << " ";

    delete[] array;
}

//Бинарная сортировка по старшему биту
void binary_quick_sort(unsigned long long *array, int first, int last, int binary_word) {
    int i = first;
    int j = last;
    if (last <= first || binary_word < 0)
        return;

    while(j != i) {
        while (!is_unit(array[i], binary_word) && (i < j) )
            ++i;
        while (is_unit(array[j], binary_word) && (j > i) )
            --j;
        std::swap(array[i], array[j]);
    }

    if ( !is_unit(array[last], binary_word) )
        ++j;

    binary_quick_sort(array, first, j - 1, binary_word - 1);
    binary_quick_sort(array, j, last, binary_word - 1);
}

//Проверка наличия единицы в раряде под номером индекс
bool is_unit(unsigned long long number, int index) {
    return number & ((unsigned long long int)1 << index);
}
