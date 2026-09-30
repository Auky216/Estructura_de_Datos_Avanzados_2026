#include <iostream>
#include "MinHeap.h"

using namespace std;

int main() {
    MinHeap heap(10);

    heap.insert(20);
    heap.insert(5);
    heap.insert(15);
    heap.insert(2);
    heap.insert(8);
    heap.insert(30);
    heap.insert(1);

    cout << "Altura del heap: " << heap.height() << endl;

    cout << "Extrayendo elementos:" << endl;

    while (!heap.empty()) {
        cout << heap.extractMin() << " ";
    }

    cout << endl;

    cout << "Altura despues de vaciar: "
         << heap.height() << endl;

    return 0;
}
