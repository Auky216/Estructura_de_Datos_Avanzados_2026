#include <cassert>
#include <iostream>

#include "MinHeap.h"
#include "PairingHeap.h"

void testInsertAndExtractOrder() {
    PairingHeap heap(5);
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
    PairingHeap heap(3);
    heap.insert(3, 7);
    heap.insert(1, 7);
    heap.insert(2, 7);

    assert(heap.extractMin() == 1);
    assert(heap.extractMin() == 2);
    assert(heap.extractMin() == 3);
}

void testDecreaseKeyMovesNodeToRoot() {
    PairingHeap heap(4);
    heap.insert(1, 20);
    heap.insert(2, 30);
    heap.insert(3, 40);
    heap.insert(4, 50);

    heap.decreaseKey(4, 10);

    assert(heap.extractMin() == 4);
    assert(heap.extractMin() == 1);
}

void testDecreaseKeyPreservesTieBreak() {
    PairingHeap heap(3);
    heap.insert(1, 10);
    heap.insert(2, 30);
    heap.insert(3, 20);

    heap.decreaseKey(2, 10);

    assert(heap.extractMin() == 1);
    assert(heap.extractMin() == 2);
}

void testDecreaseKeyTieCanCutById() {
    PairingHeap heap(2);
    heap.insert(2, 5);
    heap.insert(1, 10);

    heap.decreaseKey(1, 5);

    assert(heap.extractMin() == 1);
    assert(heap.extractMin() == 2);
    assert(heap.extractMin() == -1);
}

void testArrayMinHeapOperations() {
    MinHeap heap(6);
    heap.insert(3, 30);
    heap.insert(1, 10);
    heap.insert(2, 20);
    heap.insert(4, 40);

    heap.decreaseKey(4, 5);
    assert(heap.extractMin() == 4);
    assert(heap.extractMin() == 1);

    // Al empatar las llaves, gana el identificador menor.
    heap.decreaseKey(3, 20);
    assert(heap.extractMin() == 2);
    assert(heap.extractMin() == 3);
    assert(heap.empty());
    assert(heap.extractMin() == -1);
}

int main() {
    testInsertAndExtractOrder();
    testTieBreaksBySmallerId();
    testDecreaseKeyMovesNodeToRoot();
    testDecreaseKeyPreservesTieBreak();
    testDecreaseKeyTieCanCutById();
    testArrayMinHeapOperations();

    std::cout << "Todas las pruebas pasaron.\n";
    return 0;
}
