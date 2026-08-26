#include <iostream>

using namespace std;

class MinHeap {
private:
    int* array;
    int size;
    int capacity;

public:
    MinHeap(int n){
        this->capacity = n;
        this->size = 0;
        this->array = new int[n];
    }


    void insert(int value) {
        if (this->size == this->capacity) {
            return;
        }

        int currentIndex = this->size;
        this->array[currentIndex] = value;
        this->size = this->size + 1;

        while (currentIndex > 0) {
            int parentIndex = (currentIndex - 1) / 2;

            if (this->array[parentIndex] <= this->array[currentIndex]) {
                break;
            }

            int temporary = this->array[currentIndex];
            this->array[currentIndex] = this->array[parentIndex];
            this->array[parentIndex] = temporary;

            currentIndex = parentIndex;
        }
    }

    int extractMin() {
        if (this->size == 0) {
            return -1;
        }

        int minimum = this->array[0];

        this->size = this->size - 1;
        this->array[0] = this->array[this->size];

        int currentIndex = 0;

        while (true) {
            int leftChild = 2 * currentIndex + 1;
            int rightChild = 2 * currentIndex + 2;
            int smallestIndex = currentIndex;

            if (leftChild < this->size &&
                this->array[leftChild] < this->array[smallestIndex]) {
                smallestIndex = leftChild;
            }

            if (rightChild < this->size &&
                this->array[rightChild] < this->array[smallestIndex]) {
                smallestIndex = rightChild;
            }

            if (smallestIndex == currentIndex) {
                break;
            }

            int temporary = this->array[currentIndex];
            this->array[currentIndex] = this->array[smallestIndex];
            this->array[smallestIndex] = temporary;

            currentIndex = smallestIndex;
        }

        return minimum;
    }

    int height() {
        if (this->size == 0) {
            return -1;
        }

        int heapHeight = 0;
        int nodesAtLastLevel = this->size;

        while (nodesAtLastLevel > 1) {
            nodesAtLastLevel = nodesAtLastLevel / 2;
            heapHeight = heapHeight + 1;
        }

        return heapHeight;
    }
};
