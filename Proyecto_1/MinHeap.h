#pragma once

#include <stdexcept>
#include <utility>

struct MinHeapNode {
    int id;
    long long key;

    MinHeapNode() : id(0), key(0) {}
    MinHeapNode(int nodeId, long long nodeKey) : id(nodeId), key(nodeKey) {}
};

class MinHeap {
private:
    MinHeapNode* array;
    int* position;
    int numberOfElements;
    int capacity;

    bool hasHigherPriority(
        const MinHeapNode& first,
        const MinHeapNode& second
    ) const {
        if (first.key != second.key) {
            return first.key < second.key;
        }
        return first.id < second.id;
    }

    void swapNodes(int first, int second) {
        std::swap(array[first], array[second]);
        position[array[first].id] = first;
        position[array[second].id] = second;
    }

    void bubbleUp(int index) {
        while (index > 1) {
            int parent = index / 2;
            if (!hasHigherPriority(array[index], array[parent])) {
                return;
            }
            swapNodes(index, parent);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index;
            int right = left + 1;
            int best = index;

            if (left <= numberOfElements &&
                hasHigherPriority(array[left], array[best])) {
                best = left;
            }
            if (right <= numberOfElements &&
                hasHigherPriority(array[right], array[best])) {
                best = right;
            }
            if (best == index) {
                return;
            }

            swapNodes(index, best);
            index = best;
        }
    }

public:
    explicit MinHeap(int maximumElements)
        : array(nullptr),
          position(nullptr),
          numberOfElements(0),
          capacity(maximumElements) {
        if (maximumElements < 0) {
            throw std::invalid_argument("La capacidad no puede ser negativa");
        }

        array = new MinHeapNode[maximumElements + 1];
        position = new int[maximumElements + 1];
        for (int id = 0; id <= capacity; ++id) {
            position[id] = -1;
        }
    }

    ~MinHeap() {
        delete[] array;
        delete[] position;
    }

    MinHeap(const MinHeap&) = delete;
    MinHeap& operator=(const MinHeap&) = delete;

    bool insert(int id, long long key) {
        if (numberOfElements == capacity || id < 1 || id > capacity ||
            position[id] != -1) {
            return false;
        }

        ++numberOfElements;
        array[numberOfElements] = MinHeapNode(id, key);
        position[id] = numberOfElements;
        bubbleUp(numberOfElements);
        return true;
    }

    int extractMin() {
        if (numberOfElements == 0) {
            return -1;
        }

        int minimumId = array[1].id;
        position[minimumId] = -1;

        if (numberOfElements == 1) {
            --numberOfElements;
            return minimumId;
        }

        array[1] = array[numberOfElements];
        position[array[1].id] = 1;
        --numberOfElements;
        heapifyDown(1);
        return minimumId;
    }

    int getMin() const {
        return numberOfElements == 0 ? -1 : array[1].id;
    }

    bool decreaseKey(int id, long long newKey) {
        if (id < 1 || id > capacity || position[id] == -1) {
            return false;
        }

        int index = position[id];
        if (newKey > array[index].key) {
            return false;
        }

        array[index].key = newKey;
        bubbleUp(index);
        return true;
    }

    bool empty() const {
        return numberOfElements == 0;
    }

    int size() const {
        return numberOfElements;
    }
};
