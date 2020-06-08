#include <iostream>
#include <queue>
#include <stack>

struct naive_tree_node {
    int key;
    naive_tree_node *left, *right;

    naive_tree_node(const int &data) : key(data), left(nullptr), right(nullptr) {}
    ~naive_tree_node() {
        delete left;
        delete right;
    }
};

template <typename Comparator>
class Naive_Tree {
public:
    Naive_Tree(): root(nullptr) {}
    ~Naive_Tree() {}

    bool Add(const int &key) {
        auto *new_node = new naive_tree_node(key);
        Comparator cmp = Comparator();

        if (root == nullptr) {
            root = new_node;
            return true;
        }

        naive_tree_node *next_node = root;

        while (true) {
            if (cmp(key, next_node->key)){
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

    naive_tree_node & get_root_node() {return *root;}

private:
    naive_tree_node *root;
};


struct treap_node {
    int data;
    int priority;
    treap_node *left, *right;

    treap_node(const int &data, const int &priority) : data(data), left(nullptr), right(nullptr), priority(priority) {}
    ~treap_node() {
        delete left;
        delete right;
    }
};

class Treap_Tree{
public:
    Treap_Tree() : root(nullptr) {}
    ~Treap_Tree() {}

    void Add(const int &key, int priority) {
        treap_node *node = new treap_node(key, priority),
                *current_node = root,
                *previous_node = root;

        if(current_node == nullptr) {
            set_root_node(node);
            return;
        }


        while(current_node != nullptr && priority <= current_node->priority) {
            previous_node = current_node;
            if(key <= current_node->data)
                current_node = current_node->left;
            else
                current_node = current_node->right;
        }


        split(current_node, key, node->left, node->right);


        if(current_node == root)
            root = node;
        else if(key <= previous_node->data)
            previous_node->left = node;
        else
            previous_node->right = node;
    }

    treap_node & get_root_node() {return *root;}
private:
    treap_node *root;

    void set_root_node(treap_node *_root) { root = _root;}

    void split(treap_node *current_node, const int &data, treap_node *&left, treap_node *&right) {
        if(current_node == nullptr) {
            left = nullptr;
            right = nullptr;
        }
        else if(current_node->data <= data) {
            split(current_node->right, data, current_node->right, right);
            left = current_node;
        }
        else {
            split(current_node->left, data, left, current_node->left);
            right = current_node;
        }
    }
};

template <typename node_t>
int get_width(node_t *root_node) {
    int max_width = 0,
            current_width = 0;

    std::queue<node_t *> layer_nodes;
    node_t *current_node = nullptr;
    layer_nodes.push(root_node);

    while(!layer_nodes.empty()) {
        if((current_width = layer_nodes.size()) > max_width) {
            max_width = current_width;
        }

        for(size_t i = 0; i < current_width; i++) {
            current_node = layer_nodes.front();
            if(current_node->left != nullptr) {
                layer_nodes.push(current_node->left);
            }
            if(current_node->right != nullptr) {
                layer_nodes.push(current_node->right);
            }
            layer_nodes.pop();
        }
    }

    return max_width;
}

int main()
{
    // N - количество элементов в дереве
    size_t N;
    std::cin >> N;

    int key, priority;

    Naive_Tree<std::less<>> naive_tree;
    Treap_Tree treap_tree;


    for (size_t i = 0; i < N; ++i) {
        std::cin >> key >> priority;
        naive_tree.Add(key);
        treap_tree.Add(key, priority);
    }

    int width_difference = get_width(&treap_tree.get_root_node()) - get_width(&naive_tree.get_root_node());
    std::cout << width_difference << std::endl;

    return 0;
}
