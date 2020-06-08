/*
 *
 * Придиус Екатерина, АПО-13
 *
 * Задача 1_1
 *
    Даны два массива целых чисел одинаковой длины A[0..n-1] и B[0..n-1].
    Необходимо найти первую пару индексов i0 и j0, i0 ≤ j0,
    такую что A[i0] + B[j0] = max {A[i] + B[j], где 0 <= i < n, 0 <= j < n, i <= j}.
    Требования: Время работы - O(n).
    n ≤ 100000.

    in              out
    4               0 1
    4 -8 6 0
    -10 3 1 1

*/



#include <iostream>
#include <vector>
#include <limits>

void getMaxPositions(const int *A, int n, int *a_max_positions);

void getMaxSum(const int *A, const int *B, const int *a_max_positions, int n);

int main()
{
    int n = 0;
    std::cin >> n;
    int *A = new int[n];
    int *B = new int[n];

    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        A[i] = value;
    }

    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        B[i] = value;
    }

    int *a_max_positions = new int[n];
    getMaxPositions(A, n, a_max_positions);
    getMaxSum(A, B, a_max_positions, n);

    delete [] a_max_positions;
    delete [] A;
    delete [] B;

    return 0;
}


// Создаем массив с максимумами из массива A для каждого элемента массива B
// Это делается для того, чтобы если A[i + 1] < A[i], брать для сравнения только значение A[i],
// поскольку сумма с теми же элементами массива B, априори с ним будет больше (A[i] + B[i] > A[i + 1] + B[i])
void getMaxPositions(const int *A, int n, int *a_max_positions) {
    int max_value = std::numeric_limits<int>::min();
    // std::vector<int> a_max_positions(n);
    int current_max_value = 0;
    for (int i = 0; i < n; ++i) {
        if (A[i] > max_value) {
            max_value = A[i];
            current_max_value = i;
        }
        a_max_positions[i] = current_max_value;
    }
}


// Проходим по массиву B, ищем наибольшую сумму
void getMaxSum(const int *A, const int *B, const int *a_max_positions, int n) {
    int first_pos = 0, second_pos = 0;
    int current_max_value = 0;
    int max_value = std::numeric_limits<int>::min();
    for (int i = 0; i < n; ++i) {
        current_max_value = B[i] + A[a_max_positions[i]];
        if (current_max_value > max_value) { //берем строго больше, поскольку ищем первые индексы для максимальной суммы
            max_value = current_max_value;
            first_pos = a_max_positions[i];
            second_pos = i;
        }
    }
    std::cout << first_pos << " " << second_pos << std::endl;
}
