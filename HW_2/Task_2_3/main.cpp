/*
 *
 * Придиус Екатерина, АПО-13
 *
 * Задача 3_1
 *
 *  Дано число N < 106 и последовательность пар целых чисел из [-231..231] длиной N.
    Построить декартово дерево из N узлов, характеризующихся парами чисел {Xi, Yi}.
    Каждая пара чисел {Xi, Yi} определяет ключ Xi и приоритет Yi в декартовом дереве.
    Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
    При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
    Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x,
    а во втором больше или равны x. Получившиеся два дерева сделайте дочерними для нового узла (x, y).
    Новый узел вставьте на место узла P.

    Построить также наивное дерево поиска по ключам Xi методом из задачи 2.

    Вычислить разницу глубин наивного дерева поиска и декартового дерева. Разница может быть отрицательна.
*/

#include <iostream>
#include <queue>
#include <stack>

/** Структура узла наивного бинарного дерева поиска **/
struct Node {
    int data;
    Node *left, *right;

    Node(const int &data) : data(data), left(nullptr), right(nullptr) {}
};

/** Структура узла наивного декартова дерева **/
struct TreapNode {
    int data;
    int priority;
    TreapNode *left, *right;

    TreapNode(const int &data, const int &priority) : data(data), left(nullptr), right(nullptr), priority(priority) {}
};

/** Класс наивного бинарного дерева поиска **/
template <typename Comparator>
class BinaryTree {
public:
   BinaryTree(): root(nullptr) {}
   ~BinaryTree();

   bool Add(const int &data);

   size_t GetDepth();
private:
    Node *root;
};

/** Класс декартова дерева **/
class TreapTree{
public:
    TreapTree() : root(nullptr) {}
    ~TreapTree() { DestroyTree(root);}

    void Add(const int &data, int priority);

    size_t GetDepth(TreapNode *node);

    TreapNode & GetRoot() {return *root;}
private:
    TreapNode *root;

    void SetRoot(TreapNode *_root) { root = _root;}

    void Split(TreapNode *current_node, const int &data, TreapNode *&left, TreapNode *&right);\

    void DestroyTree(TreapNode *node);
};

///------------------------------------------------------------

int main()
{
    // N - количество элементов в дереве
    size_t N;
    std::cin >> N;

    int key, priority;

    BinaryTree<std::less<>> binaryTree;
    TreapTree treapTree;


    for (size_t i = 0; i < N; ++i) {
        std::cin >> key;
        std::cin >> priority;
        binaryTree.Add(key);
        treapTree.Add(key, priority);
    }

    size_t treeDepth = binaryTree.GetDepth();
    size_t treapDepth = treapTree.GetDepth(&treapTree.GetRoot());

    std::cout << treeDepth - treapDepth << std::endl;

    return 0;
}

///------------------------------------------------------------

/*****  Реализация декартова дерева  *****/

/** Рекурсивный поиск максимальной глубины декартова дерева **/
size_t TreapTree::GetDepth(TreapNode *node) {
    if (node == nullptr) return 0;

    size_t depth = 0;
    size_t left_depth = GetDepth(node->left);
    size_t right_depth = GetDepth(node->right);
    depth = std::max(left_depth + 1, right_depth + 1);

    return depth;
}

/** Добавление элемента в декартово дерево **/
void TreapTree::Add(const int &data, int priority) {
    TreapNode *node = new TreapNode(data, priority),
            *current_node = root,
            *previous_node = root;

    if(current_node == nullptr) {
        SetRoot(node);
        return;
    }


    while(current_node != nullptr && priority <= current_node->priority) {
        previous_node = current_node;
        if(data <= current_node->data)
            current_node = current_node->left;
        else
            current_node = current_node->right;
    }


    Split(current_node, data, node->left, node->right);


    if(current_node == root)
        root = node;
    else if(data <= previous_node->data)
        previous_node->left = node;
    else
        previous_node->right = node;
}

/** Служебная функция для декартова дерево **/
void TreapTree::Split(TreapNode *current_node, const int &data, TreapNode *&left, TreapNode *&right) {
    if(current_node == nullptr) {
        left = nullptr;
        right = nullptr;
    }
    else if(current_node->data <= data) {
        Split(current_node->right, data, current_node->right, right);
        left = current_node;
    }
    else {
        Split(current_node->left, data, left, current_node->left);
        right = current_node;
    }
}

void TreapTree::DestroyTree(TreapNode *node) {
    if (node) {
        DestroyTree(node->right);
        DestroyTree(node->left);
        delete node;
    }
}

/*****  Реализация бинарного дерева поиска  *****/

/** Нерекурсивный поиск максимальной глубины наивного бинарного дерева поиска **/
template<typename Comparator>
size_t BinaryTree<Comparator>::GetDepth() {
    Node *node = root;
    if (node == nullptr) return 0;

    std::stack<Node *> s;
    std::stack<size_t> depths;

    int current_depth = 0;
    int max_depth = 0;

    while (true){
        ++current_depth;

        if (node->left != nullptr) {
            if (node->right != nullptr) { s.push(node->right); depths.push(current_depth); }
            node = node->left;
        } else if (node->right != nullptr) {
            node = node->right;
        } else {
            if (current_depth > max_depth) max_depth = current_depth;
            if (s.empty()) break;

            node = s.top();
            s.pop();

            current_depth = depths.top();
            depths.pop();
        }
    }

    return max_depth;
}

/** Добавление элемента в наивное бинарное дерево поиска **/
template <typename Comparator>
bool BinaryTree<Comparator>::Add(const int &data) {
    Node *new_node = new Node(data);
    Comparator cmp = Comparator();

    if (root == nullptr) {
        root = new_node;
        return true;
    }

    Node *next_node = root;

    while (true) {
        if (cmp(data, next_node->data)){
            if (next_node->left != nullptr)
                next_node = next_node->left;
            else {
                next_node->left = new_node;
                return true;
            }
        }
        else {
            if (next_node->right != nullptr)
                next_node = next_node->right;
            else {
                next_node->right = new_node;
                return true;
            }
        }
    }
}
template<typename Comparator>
BinaryTree<Comparator>::~BinaryTree() {
    std::queue<Node *> tmp;
    if (root)
        tmp.push(root);

    while (!tmp.empty()) {
        Node *current = tmp.front();
        if (current->left)
            tmp.push(current->left);
        if (current->right)
            tmp.push(current->right);

        delete current;
        tmp.pop();
    }
}
