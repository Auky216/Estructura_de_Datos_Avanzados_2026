#pragma once

#include <iostream>

struct Node {
    int id;
    int value;

    Node() {
        this->id = 0;
        this->value = 0;
    }

    Node(int id, int value) {
        this->id = id;
        this->value = value;
    }
};

class MinHeap {
private:
    Node* array;
    int* position;
    int size;
    int capacity;

    bool hasHigherPriority(const Node& first, const Node& second) {
        if (first.value != second.value) {
            return first.value < second.value;
        }
        return first.id < second.id;
    }

    void swapNodes(int first, int second) {
        Node temp = this->array[first];
        this->array[first] = this->array[second];
        this->array[second] = temp;

        this->position[this->array[first].id] = first;
        this->position[this->array[second].id] = second;
    }

    void bubbleUp(int index) {
        while (index > 1) {
            int parent = index / 2;
            if (!hasHigherPriority(this->array[index], this->array[parent])) {
                break;
            }
            swapNodes(index, parent);
            index = parent;
        }
    }

    void minHeapify(int index) {
        while (true) {
            int left = 2 * index;
            int right = left + 1;
            int minimum = index;

            if (left <= this->size &&
                hasHigherPriority(this->array[left], this->array[minimum])) {
                minimum = left;
            }
            if (right <= this->size &&
                hasHigherPriority(this->array[right], this->array[minimum])) {
                minimum = right;
            }
            if (minimum == index) {
                return;
            }

            swapNodes(index, minimum);
            index = minimum;
        }
    }

public:
    MinHeap(int maximumElements) {
        this->capacity = maximumElements;
        this->size = 0;
        this->array = new Node[maximumElements + 1];
        this->position = new int[maximumElements + 1];

        for (int id = 0; id <= maximumElements; id++) {
            this->position[id] = -1;
        }
    }

    ~MinHeap() {
        delete[] this->array;
        delete[] this->position;
    }

    void insert(int id, int value) {
        if (this->size == this->capacity) {
            return;
        }

        this->size++;
        this->array[this->size] = Node(id, value);
        this->position[id] = this->size;
        bubbleUp(this->size);
    }

    int extractMin() {
        int minimumId = this->array[1].id;
        this->position[minimumId] = -1;

        this->array[1] = this->array[this->size];
        this->size--;

        if (this->size > 0) {
            this->position[this->array[1].id] = 1;
            minHeapify(1);
        }

        return minimumId;
    }

    void decreaseKey(int id, int newValue) {
        int index = this->position[id];
        this->array[index].value = newValue;
        bubbleUp(index);
    }

    void print() {
        for (int index = 1; index <= this->size; index++) {
            std::cout << "(" << this->array[index].id << ", "
                      << this->array[index].value << ") ";
        }
        std::cout << '\n';
    }
};
