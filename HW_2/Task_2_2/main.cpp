/*
 *
 * Придиус Екатерина, АПО-13
 *
 * Задача 1_2
 *
 *  Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
    Требуется построить бинарное дерево, заданное наивным порядком вставки.
    Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
    то узел K добавляется в правое поддерево root; иначе в левое поддерево root.

    Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.
*/

#include <iostream>
#include <queue>
#include <stack>

struct Node {
    int data;
    Node *left, *right;

    Node(const int &data) : data(data), left(nullptr), right(nullptr) {}
};

template <typename Comparator>
class BinaryTree {
public:
   BinaryTree(): root(nullptr) {}
   ~BinaryTree();

   bool Add(const int &data);

   void PreOrder();

    int get_min_depth(Node *node);

    Node & get_root() {return *root;}
private:
    Node *root;
};


int main()
{
    // N - количество элементов в дереве
    size_t N;
    std::cin >> N;

    // buf - число для добавления
    int buf;

    BinaryTree<std::less<>> tree;
    // Вводим оставшиеся значения и добавляем в дерево
    for (size_t i = 0; i < N; ++i) {
        std::cin >> buf;
        tree.Add(buf);
    }


   // std::cout << tree.get_min_depth(&tree.get_root());

    tree.PreOrder();
    return 0;
}

template <typename Comparator>
bool BinaryTree<Comparator>::Add(const int &data) {
    Node *newNode = new Node(data);
    Comparator cmp = Comparator();

    if (root == nullptr) {
        root = newNode;
        return true;
    }

    Node *nextNode = root;

    while (true) {
        if (cmp(data,nextNode->data)){
            // Смотрим налево
            if (nextNode->left != nullptr)
                nextNode = nextNode->left;
            else {
                nextNode->left = newNode;
                return true;
            }
        }
        else {
            // Смотрим направо
            if (nextNode->right != nullptr)
                nextNode = nextNode->right;
            else {
                nextNode->right = newNode;
                return true;
            }
        }
    }
}

template <typename Comparator>
void BinaryTree<Comparator>::PreOrder() {
    Node *node = root;

    std::stack<Node *> s;
    s.push(node);
    while (!s.empty()) {
        node = s.top();
        s.pop();
        std::cout << node->data << " ";
        if (node->right != nullptr)
            s.push(node->right);
        if (node->left != nullptr)
            s.push(node->left);
    }
}


template <typename Comparator>
int BinaryTree<Comparator>::get_min_depth(Node *node) {
    int ret = 0;
    if (node != nullptr) {
        int left_depth = get_min_depth(node->left);
        int right_depth = get_min_depth(node->right);
        ret = std::min(left_depth + 1, right_depth + 1);
    }

    return ret;
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