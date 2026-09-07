#pragma once

struct MinHeapNode {
    int id;
    long long key;

    MinHeapNode() {
        this->id = 0;
        this->key = 0;
    }

    MinHeapNode(int nodeId, long long nodeKey) {
        this->id = nodeId;
        this->key = nodeKey;
    }
};

class MinHeap {
private:
    MinHeapNode* array;
    int size;
    int capacity;

    bool hasHigherPriority(const MinHeapNode& first,
                           const MinHeapNode& second) {
        if (first.key != second.key) {
            return first.key < second.key;
        }

        return first.id < second.id;
    }

    void bubbleUp(int index) {
        while (index > 1) {
            int parent = index / 2;

            if (!hasHigherPriority(this->array[index], this->array[parent])) {
                break;
            }

            MinHeapNode temporary = this->array[index];
            this->array[index] = this->array[parent];
            this->array[parent] = temporary;
            index = parent;
        }
    }

public:
    MinHeap(int maximumElements) {
        this->size = 0;
        this->capacity = maximumElements;
        this->array = new MinHeapNode[maximumElements + 1];
    }

    ~MinHeap() {
        delete[] this->array;
    }

    void insert(int id, long long key) {
        if (this->size == this->capacity) {
            return;
        }

        this->size = this->size + 1;
        this->array[this->size] = MinHeapNode(id, key);

        bubbleUp(this->size);
    }
};
