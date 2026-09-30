#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdexcept>
#include <utility>

class MinHeap {
private:
    int* array;
    int numberOfElements;
    int capacity;

    void heapifyDown(int index) {
        while (true) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int smallest = index;

            if (leftChild < numberOfElements &&
                array[leftChild] < array[smallest]) {
                smallest = leftChild;
            }
            if (rightChild < numberOfElements &&
                array[rightChild] < array[smallest]) {
                smallest = rightChild;
            }
            if (smallest == index) {
                return;
            }

            std::swap(array[index], array[smallest]);
            index = smallest;
        }
    }

public:
    explicit MinHeap(int maximumElements)
        : array(nullptr), numberOfElements(0), capacity(maximumElements) {
        if (maximumElements < 0) {
            throw std::invalid_argument("La capacidad no puede ser negativa");
        }
        array = new int[maximumElements];
    }

    ~MinHeap() {
        delete[] array;
    }

    MinHeap(const MinHeap&) = delete;
    MinHeap& operator=(const MinHeap&) = delete;

    bool insert(int value) {
        if (numberOfElements == capacity) {
            return false;
        }

        int index = numberOfElements++;
        array[index] = value;

        while (index > 0) {
            int parent = (index - 1) / 2;
            if (array[parent] <= array[index]) {
                break;
            }
            std::swap(array[parent], array[index]);
            index = parent;
        }
        return true;
    }

    int getMin() const {
        return numberOfElements == 0 ? -1 : array[0];
    }

    int extractMin() {
        if (numberOfElements == 0) {
            return -1;
        }

        int minimum = array[0];
        --numberOfElements;
        if (numberOfElements > 0) {
            array[0] = array[numberOfElements];
            heapifyDown(0);
        }
        return minimum;
    }

    bool empty() const {
        return numberOfElements == 0;
    }

    int size() const {
        return numberOfElements;
    }

    int height() const {
        if (numberOfElements == 0) {
            return -1;
        }

        int result = 0;
        for (int nodes = numberOfElements; nodes > 1; nodes /= 2) {
            ++result;
        }
        return result;
    }
};

#endif
