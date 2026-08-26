#include <iostream>

// PairingHeap es la estructura predeterminada del proyecto.
// Para ejecutar la alternativa MinHeap, compilar con -DUSE_MIN_HEAP.
#ifdef USE_MIN_HEAP
#include "MinHeap.h"
using ActiveHeap = MinHeap;
#else
#include "PairingHeap.h"
using ActiveHeap = PairingHeap;
#endif

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int q;
    cin >> n >> q;

    ActiveHeap heap(n + q);

    for (int id = 1; id <= n; id++) {
        long long key;
        cin >> key;
        heap.insert(id, key);
    }

    int nextId = n + 1;

    for (int query = 0; query < q; query++) {
        int type;
        cin >> type;

        if (type == 1) {
            long long key;
            cin >> key;
            heap.insert(nextId, key);
            nextId++;
        } else if (type == 2) {
            cout << heap.extractMin() << '\n';
        } else if (type == 3) {
            int id;
            long long newKey;
            cin >> id >> newKey;
            heap.decreaseKey(id, newKey);
        }
    }

    return 0;
}
