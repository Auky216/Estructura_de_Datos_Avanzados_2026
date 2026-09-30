#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <vector>

struct Node {
    int value;
    Node* left;
    Node* right;

    explicit Node(int value) : value(value), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    Node* root;
    int numberOfElements;

    Node* remove(Node* current, int value, bool& removed) {
        if (current == nullptr) {
            return nullptr;
        }

        if (value < current->value) {
            current->left = remove(current->left, value, removed);
        } else if (value > current->value) {
            current->right = remove(current->right, value, removed);
        } else {
            removed = true;

            if (current->left == nullptr) {
                Node* rightChild = current->right;
                delete current;
                return rightChild;
            }

            if (current->right == nullptr) {
                Node* leftChild = current->left;
                delete current;
                return leftChild;
            }

            Node* successor = current->right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }

            current->value = successor->value;
            bool ignored = false;
            current->right = remove(current->right, successor->value, ignored);
        }

        return current;
    }

    void inorder(Node* current, std::vector<int>& values) const {
        if (current == nullptr) {
            return;
        }

        inorder(current->left, values);
        values.push_back(current->value);
        inorder(current->right, values);
    }

    void clear(Node* current) {
        if (current == nullptr) {
            return;
        }

        clear(current->left);
        clear(current->right);
        delete current;
    }

public:
    BinaryTree() : root(nullptr), numberOfElements(0) {}

    ~BinaryTree() {
        clear(root);
    }

    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    void insert(int value) {
        Node** current = &root;

        while (*current != nullptr) {
            if (value < (*current)->value) {
                current = &((*current)->left);
            } else {
                // Los duplicados se guardan en el subarbol derecho.
                current = &((*current)->right);
            }
        }

        *current = new Node(value);
        ++numberOfElements;
    }

    bool contains(int value) const {
        Node* current = root;

        while (current != nullptr) {
            if (value == current->value) {
                return true;
            }
            current = value < current->value
                ? current->left
                : current->right;
        }

        return false;
    }

    bool remove(int value) {
        bool removed = false;
        root = remove(root, value, removed);
        if (removed) {
            --numberOfElements;
        }
        return removed;
    }

    std::vector<int> inorder() const {
        std::vector<int> values;
        values.reserve(numberOfElements);
        inorder(root, values);
        return values;
    }

    bool empty() const {
        return root == nullptr;
    }

    int size() const {
        return numberOfElements;
    }
};

#endif
