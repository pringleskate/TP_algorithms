#include <iostream>
#include <vector>
#include <stack>


class AVLTree {
    struct Node {
        Node(const size_t &data) : data(data), height(1), left(nullptr), right(nullptr), number(1) {}
        size_t data;
        size_t height;
        size_t number;
        Node* left;
        Node* right;
    };
public:
    AVLTree() : root(nullptr) {}
    ~AVLTree()
    {
        destroyTree(root);
    }

    void Add(const size_t &data, size_t &position)
    {
        root = addInternal(root, data, position);
    }

    void Remove(size_t &position)
    {
        root = removeInternal(root, position);
    }
private:
    Node *root;

    size_t getHeight(Node* node)
    {
        return node ? node->height : 0;
    }

    int getBalance(Node* node)
    {
        return getHeight(node->right) - getHeight(node->left);
    }

    void fixHeight(Node* node)
    {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    void fixNumber(Node * node) {
        node->number = ((node->right == nullptr) ? 0 : node->right->number) + ((node->left == nullptr) ? 0 : node->left->number) + 1;
    }

    Node* rotateRight(Node* node)
    {
        Node* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;

        fixNumber(node);
        fixNumber(tmp);
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    Node* rotateLeft(Node* node)
    {
        Node* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        fixNumber(node);
        fixNumber(tmp);
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    Node* doBalance(Node* node)
    {
        fixHeight(node);

        switch (getBalance(node))
        {
            case 2:
                if(getBalance(node->right) < 0 )
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            case -2:
                if(getBalance(node->left) > 0  )
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            default:
                return node;
        }
    }

    Node* addInternal(Node* node, const size_t &data, size_t &position)
    {
        if(node == nullptr) {
            return new Node(data);
        }
        ++(node->number);
        if(data < node->data ) {
            position += ((node->right == nullptr) ? 0 : node->right->number) + 1;
            node->left = addInternal(node->left, data, position);
        }
        else{
            node->right = addInternal(node->right, data, position);
        }
        return doBalance(node);
    }

    Node* findAndRemoveMin(Node *node, Node *&min)
    {
        if (!node->left)
        {
            min = node;
            return node->right;
        }
        node->left = findAndRemoveMin(node->left, min);
        --node->number;
        return doBalance(node);
    }

    Node* findAndRemoveMax(Node *node, Node *&max)
    {
        if (!node->right)
        {
            max = node;
            return node->left;
        }
        node->right = findAndRemoveMax(node->right, max);
        --node->number;
        return doBalance(node);
    }

    Node* findMin(Node* node)
    {
        while (node->left)
            node = node->left;
        return node;
    }

    Node* removeMin(Node* node)
    {
        if(node->left == nullptr)
            return node->right;
        node->left = removeMin(node->left);
        --node->number;
        return doBalance(node);
    }

    Node* removeInternal(Node* node, size_t position)
    {
        if(node == nullptr)
            return nullptr;
        if (position >= node->number)
            return node;
        size_t sum = 0;
        std::stack<Node *> nodes;

        while (true)
        {
            size_t number_right = (node->right == nullptr) ? 0 : node->right->number;

            if (position - sum > number_right)
            {
                nodes.push(node);
                node = node->left;
                sum += number_right + 1;
            }
            else if (position - sum < number_right)
            {

                if (node->right != nullptr) {
                    nodes.push(node);
                    node = node->right;
                }
                else break;
            }
            else
            {
                Node* left = node->left;
                Node* right = node->right;
                size_t key = node->data;
                delete node;
                if (right == nullptr)
                {
                    if (left == nullptr)
                    {
                        if (!nodes.empty())
                        {
                            node = nodes.top();
                            nodes.pop();

                            if (node->data > key)
                                node->left = nullptr;
                            else
                                node->right = nullptr;

                            --node->number;
                        }
                        else
                            return nullptr;
                    }
                    else
                        node = left;
                }
                else
                {
                    Node *newRoot = nullptr;
                    if (getHeight(right) > getHeight(left))
                    {
                        Node *newRight = nullptr;
                        newRight = findAndRemoveMin(right, newRoot);
                        newRoot->right = newRight;
                        newRoot->left = left;
                    }
                    else
                    {
                        Node *newLeft = nullptr;
                        newLeft = findAndRemoveMax(left, newRoot);
                        newRoot->left = newLeft;
                        newRoot->right = right;
                    }

                    fixNumber(newRoot);
                    node = doBalance(newRoot);
                }
                break;
            }
        }
        Node *node_1;

        while (!nodes.empty())
        {
            node_1 = nodes.top();
            --node_1->number;

            if (node_1->data > node->data)
                node_1->left = node;
            else
                node_1->right = node;

            node = doBalance(node_1);
            nodes.pop();
        }
        return node;
    }

    void destroyTree(Node* node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};

int main()
{
    AVLTree avlTree;

    int N;
    std::cin >> N;
    std::vector<int> positions_result;

    size_t key;
    size_t position;
    size_t operation;

    for (int i = 0; i < N; i++)
    {
        std::cin >> operation;

       if (operation == 1)
       {
            position = 0;
            std::cin >> key;
            avlTree.Add(key, position);
            positions_result.push_back(position);
        }
        else if (operation == 2)
        {
            std::cin >> key;
            avlTree.Remove(key);
        }
    }

    for (auto res : positions_result) {
        std::cout << res << std::endl;
    }
    return 0;
}
