#include <iostream>

#include "MinHeap.h"

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    MinHeap heap(n + q);

    for (int id = 1; id <= n; id++) {
        int value;
        cin >> value;
        heap.insert(id, value);
    }

    int nextId = n + 1;
    for (int query = 0; query < q; query++) {
        int type;
        cin >> type;

        if (type == 1) {
            int value;
            cin >> value;
            heap.insert(nextId, value);
            nextId++;
        } else if (type == 2) {
            cout << heap.extractMin() << '\n';
        } else if (type == 3) {
            int id, value;
            cin >> id >> value;
            heap.decreaseKey(id, value);
        }
    }

    return 0;
}
