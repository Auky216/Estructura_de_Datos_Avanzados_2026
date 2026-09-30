#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <stdexcept>
#include <vector>

struct Node {
    long long value;
    Node* left;
    Node* right;

    Node() : value(0), left(nullptr), right(nullptr) {}
};

class SegmentTree {
private:
    Node* root;
    int n;

    Node* build(const std::vector<int>& values, int left, int right) {
        Node* node = new Node();

        if (left == right) {
            node->value = values[left];
            return node;
        }

        int middle = (left + right) / 2;
        node->left = build(values, left, middle);
        node->right = build(values, middle + 1, right);
        node->value = node->left->value + node->right->value;
        return node;
    }

    long long query(
        Node* node,
        int left,
        int right,
        int queryLeft,
        int queryRight
    ) const {
        if (queryRight < left || right < queryLeft) {
            return 0;
        }
        if (queryLeft <= left && right <= queryRight) {
            return node->value;
        }

        int middle = (left + right) / 2;
        return query(node->left, left, middle, queryLeft, queryRight) +
               query(node->right, middle + 1, right, queryLeft, queryRight);
    }

    void update(Node* node, int left, int right, int position, int value) {
        if (left == right) {
            node->value = value;
            return;
        }

        int middle = (left + right) / 2;
        if (position <= middle) {
            update(node->left, left, middle, position, value);
        } else {
            update(node->right, middle + 1, right, position, value);
        }

        node->value = node->left->value + node->right->value;
    }

    void clear(Node* node) {
        if (node == nullptr) {
            return;
        }
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    explicit SegmentTree(const std::vector<int>& values)
        : root(nullptr), n(static_cast<int>(values.size())) {
        if (n > 0) {
            root = build(values, 0, n - 1);
        }
    }

    ~SegmentTree() {
        clear(root);
    }

    SegmentTree(const SegmentTree&) = delete;
    SegmentTree& operator=(const SegmentTree&) = delete;

    long long query(int left, int right) const {
        if (left < 0 || right >= n || left > right) {
            throw std::out_of_range("Rango de Segment Tree invalido");
        }
        return query(root, 0, n - 1, left, right);
    }

    void update(int position, int value) {
        if (position < 0 || position >= n) {
            throw std::out_of_range("Posicion de Segment Tree invalida");
        }
        update(root, 0, n - 1, position, value);
    }

    int size() const {
        return n;
    }
};

#endif
