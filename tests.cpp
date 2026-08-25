#include <cassert>
#include <iostream>

#include "MinHeap.h"

void testInsertAndExtractOrder() {
    MinHeap heap(5);
    heap.insert(1, 50);
    heap.insert(2, 10);
    heap.insert(3, 30);
    heap.insert(4, 20);
    heap.insert(5, 40);

    assert(heap.extractMin() == 2);
    assert(heap.extractMin() == 4);
    assert(heap.extractMin() == 3);
    assert(heap.extractMin() == 5);
    assert(heap.extractMin() == 1);
}

void testTieBreaksBySmallerId() {
    MinHeap heap(3);
    heap.insert(3, 7);
    heap.insert(1, 7);
    heap.insert(2, 7);

    assert(heap.extractMin() == 1);
    assert(heap.extractMin() == 2);
    assert(heap.extractMin() == 3);
}

void testDecreaseKeyMovesNodeToRoot() {
    MinHeap heap(4);
    heap.insert(1, 20);
    heap.insert(2, 30);
    heap.insert(3, 40);
    heap.insert(4, 50);

    heap.decreaseKey(4, 10);

    assert(heap.extractMin() == 4);
    assert(heap.extractMin() == 1);
}

void testDecreaseKeyPreservesTieBreak() {
    MinHeap heap(3);
    heap.insert(1, 10);
    heap.insert(2, 30);
    heap.insert(3, 20);

    heap.decreaseKey(2, 10);

    assert(heap.extractMin() == 1);
    assert(heap.extractMin() == 2);
}

int main() {
    testInsertAndExtractOrder();
    testTieBreaksBySmallerId();
    testDecreaseKeyMovesNodeToRoot();
    testDecreaseKeyPreservesTieBreak();

    std::cout << "Todas las pruebas pasaron.\n";
    return 0;
}
