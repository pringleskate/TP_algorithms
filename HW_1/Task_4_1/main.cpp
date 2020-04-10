/*
 Богомолова Мария
 Группа АПО-13

 4_1

 Вовочка ест фрукты из бабушкиной корзины. В корзине лежат фрукты разной массы.
 Вовочка может поднять не более K грамм. Каждый фрукт весит не более K грамм.
 За раз он выбирает несколько самых тяжелых фруктов, которые может поднять одновременно,
 откусывает от каждого половину и кладет огрызки обратно в корзину.

 Если фрукт весит нечетное число грамм, он откусывает большую половину. Фрукт массы 1гр он съедает полностью.

 Определить за сколько подходов Вовочка съест все фрукты в корзине.

 Формат входных данных. Вначале вводится n - количество фруктов и n строк с массами фруктов. Затем K - "грузоподъемность".
 Формат выходных данных. Неотрицательное число - количество подходов к корзине.

*/

#include <iostream>
#include <vector>

// Куча, реализованная с помощью массива
class maxHeap {
private:
    int *buffer;        //буфер для хранения данных
    int buffer_capacity; //вместимость буфера (выделенная память)
    int buffer_size;    //количество элементов, реально хранимых в буфере

public:
    explicit maxHeap(int size);
    ~maxHeap() = default;

    int extractRoot();
    int peekRoot();
    void siftUp(int index);
    void siftDown(int index);
    void insert(int element);

    void printHeap();
    int getBufferSize();
};

//подсчет подходов по поеданию Вовочкой фруктов
int CountSteps(int fruit_count, int *fruits, int max_weight);

int main() {

    int fruit_count = 0;
    std::cin >> fruit_count;            //считываем количество фруктов в корзине

    int *fruits = new int[fruit_count];
    for (int i = 0; i < fruit_count; i++)
        std::cin >> fruits[i];          //считываем массы фруктов

    int max_weight = 0;
    std::cin >> max_weight;             //считываем "грузоподъемность" Вовочки

    int count_steps = CountSteps(fruit_count, fruits, max_weight); //подсчитываем количество необходимых подходов

    std::cout << count_steps;
    return 0;
}



int CountSteps(int fruit_count, int *fruits, int max_weight) {

    maxHeap fruits_heap(fruit_count);

    //создаем кучу Max Heap из значений масс фруктов
    for (int i = 0; i < fruit_count; i++)
        fruits_heap.insert(fruits[i]);


    int count_steps = 0;


    // временный буфер для хранения взятых Вовой фруктов
    std::vector<int> tmp_buffer;

    while (true) {
        int current_weight = 0; // текущий вес в руках
        while (true) {
            if (fruits_heap.getBufferSize()) { //если куча не пуста - пытаемся взять оттуда фрукты
                if ((fruits_heap.peekRoot() + current_weight) > max_weight) {
                    // если масса фрукта в корне в сумме с уже имеющимся текущим весом  не вмещается в руки Вовочке,
                    // заканчиваем подход
                    count_steps++;
                    break;
                }
                else {
                    int tmp = fruits_heap.extractRoot(); //если же фрукт в корне кучи вмещается - извлекаем ее оттуда
                    current_weight += tmp;
                    if (tmp != 1)
                        //если масса фрукта = 1, то Вова съедает его полностью, иначе - надо положить значение во временный буфер,
                        // чтобы заново добавить оставшуюся часть в кучу
                        tmp_buffer.push_back(tmp);
                }
            }
            else if (!tmp_buffer.empty()) {
                // заглушка на случай, если куча уже пуста, а фрукт > 1
                // => надо сделать еще один подход, чтобы съесть оставшуюся половину
                count_steps++;
                break;
            } else break;
        }

        while (!tmp_buffer.empty()) {
            // добавляем откусанные фрукты обратно в кучу
            fruits_heap.insert(tmp_buffer.back() / 2);
            tmp_buffer.pop_back();
        }

        if (!fruits_heap.getBufferSize()) {
            count_steps++;
            break;
        }
    }
    return count_steps;
}


maxHeap::maxHeap(int size): buffer_capacity(size), buffer_size(0) {
    buffer = new int[buffer_capacity];
}

// извлечение корневого (максимального) элемента кучи
int maxHeap::extractRoot() {
    int tmp = buffer[0];
    std::swap(buffer[0], buffer[--buffer_size]);
    siftDown(0); //создаем кучу из неупорядоченного массива данных

    return tmp;
}

// получаем значение корневого элемента в куче без его удаления
int maxHeap::peekRoot() {
    return buffer[0];
}

// проталкивание элемента вниз
void maxHeap::siftDown(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    // Ищем большего сына, если такой есть.
    int largest = index;

    if(left < buffer_size && buffer[left] > buffer[index])
        largest = left;

    if(right < buffer_size && buffer[right] > buffer[largest])
        largest = right;

    // Если больший сын есть, то проталкиваем корень в него.
    if(largest != index) {
        std::swap(buffer[index], buffer[largest]);
        siftDown(largest);
    }
}

// проталкивание элемента вверх
void maxHeap::siftUp(int index) {
    int parent;

    while(index > 0) {
        parent = (index - 1) / 2;

        if(buffer[index] <= buffer[parent])
            return;

        std::swap(buffer[index], buffer[parent]);
        index = parent;
    }
}

// добавление нового элемента в кучу
void maxHeap::insert(int element) {
    buffer[buffer_size] = element;
    siftUp(buffer_size);
    buffer_size++;
}

// получение количества хранимых в куче элементов
int maxHeap::getBufferSize() {
    return buffer_size;
}

void maxHeap::printHeap() {
    for (int i = 0; i < buffer_size; i++)
        std::cout << buffer[i] << " ";
    std::cout << std::endl;
}

