/*
 * Придиус Екатерина, АПО-13
 *
 *  Задача 2_1
    Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A,
    равного или превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет, выведите n. n, m ≤ 10000.
    Требования:  Время работы поиска k для каждого элемента B[i]: O(log(k)).
    Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k, а потом уже в нем делать бинарный поиск.

    Формат входных данных.
    В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.
    in              out
    2 1             1
    1 2
    2

    4 3             1 3 0
    2 4 5 7
    4 6 1

*/

#include <iostream>
using namespace std;

int findNearestMinValue(const int *array_A, int element, int N);

int binarySearch(const int *array, int start, int end, int element);


int main() {

    int N = 0, M = 0;
    cin >> N >> M;


    int *array_A = new int[N];
    int *array_B = new int[M];

    //считываем количество элементов в массивах A и B, после чего считываем элементы первого массива
    for (int i = 0; i < N; i++)
        cin >> array_A[i];

    //для каждого считываемого элемента второго массива ищем ближайшее минимальное значение
    for (int i = 0; i < M; i++)
    {
        cin >> array_B[i];
        cout << findNearestMinValue(array_A, array_B[i], N) << " ";
    }

    delete [] array_A;
    delete [] array_B;

    return 0;
}


int findNearestMinValue(const int *array_A, int element, int N)
{
    //обработка частных случаев
    
    //если обрабатываемый элемент меньше первого элемента в отсортированном массиве,
    //то ближайшим минимальным значеением является первый элемент
    if (element <= array_A[0])
        return 0;

    //если обрабатываемый элемент больше последнего элемента в отсортированном массиве,
    //то ближайшим минимальным значеением является последний элемент
    if (element > array_A[N - 1])
        return N;

   //обработка общего случая
   int start = 0, end = N;

   //определяем диапазон для вызова в нем бинарного поиска
   while (array_A[start] < element)
       start++;

   return binarySearch(array_A, start, end, element);
}

//реализация бинарного поиска
int binarySearch(const int *array, int start, int end, int element) {
    int middle = 0;
    while (start != end) {
        middle = (end + start) / 2;

        if (element <= array[middle]) {
            end = middle;
        } else
            start = middle + 1;
    }
    return start;
}
