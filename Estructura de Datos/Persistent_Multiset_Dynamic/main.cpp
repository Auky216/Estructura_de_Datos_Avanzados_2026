#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

const int MIN_VALUE = 0;
const int MAX_VALUE = 1000000000;

struct PersistentSegmentTreeNode {
    int count;
    int l, r;
    PersistentSegmentTreeNode* left;
    PersistentSegmentTreeNode* right;
};

class DynamicPersistentMultiset {
private:
    int nodeCount(PersistentSegmentTreeNode* node) const {
        return node == nullptr ? 0 : node->count;
    }

    PersistentSegmentTreeNode* updateNode(
        PersistentSegmentTreeNode* previous,
        int l,
        int r,
        int position,
        int delta
    ) {
        PersistentSegmentTreeNode* current;

        if (previous == nullptr) {
            current = new PersistentSegmentTreeNode{
                0, l, r, nullptr, nullptr
            }; // NUEVO NODO
        } else {
            current = new PersistentSegmentTreeNode(*previous); // NUEVO NODO
            current->left = previous->left;   // COMPARTIDO
            current->right = previous->right; // COMPARTIDO
        }

        current->count = nodeCount(previous) + delta;

        if (l == r) {
            return current;
        }

        int mid = l + (r - l) / 2;
        if (position <= mid) {
            PersistentSegmentTreeNode* previousLeft =
                previous == nullptr ? nullptr : previous->left;
            current->left = updateNode(
                previousLeft, l, mid, position, delta
            );
            // current->right sigue COMPARTIDO.
        } else {
            PersistentSegmentTreeNode* previousRight =
                previous == nullptr ? nullptr : previous->right;
            current->right = updateNode(
                previousRight, mid + 1, r, position, delta
            );
            // current->left sigue COMPARTIDO.
        }

        return current;
    }

    int frequencyAt(
        PersistentSegmentTreeNode* current,
        int l,
        int r,
        int position
    ) const {
        if (current == nullptr) {
            return 0;
        }
        if (l == r) {
            return current->count;
        }

        int mid = l + (r - l) / 2;
        if (position <= mid) {
            return frequencyAt(current->left, l, mid, position);
        }
        return frequencyAt(current->right, mid + 1, r, position);
    }

    int kthValue(
        PersistentSegmentTreeNode* current,
        int l,
        int r,
        int k
    ) const {
        if (l == r) {
            return l;
        }

        int mid = l + (r - l) / 2;
        int leftCount = nodeCount(current->left);

        if (leftCount >= k) {
            return kthValue(current->left, l, mid, k);
        }
        return kthValue(current->right, mid + 1, r, k - leftCount);
    }

public:
    vector<PersistentSegmentTreeNode*> roots;

    DynamicPersistentMultiset() {
        // nullptr representa el multiconjunto vacio sin construir 1e9 hojas.
        roots.push_back(nullptr);
    }

    // delta = +1 inserta y delta = -1 elimina una aparicion.
    PersistentSegmentTreeNode* update(
        PersistentSegmentTreeNode* previousRoot,
        int position,
        int delta
    ) {
        if (position < MIN_VALUE || position > MAX_VALUE) {
            throw out_of_range("Valor fuera de [0, 1e9]");
        }
        if (delta < 0 && frequency(previousRoot, position) + delta < 0) {
            throw invalid_argument("No se puede eliminar un valor ausente");
        }

        return updateNode(
            previousRoot, MIN_VALUE, MAX_VALUE, position, delta
        );
    }

    int frequency(
        PersistentSegmentTreeNode* currentRoot,
        int position
    ) const {
        return frequencyAt(
            currentRoot, MIN_VALUE, MAX_VALUE, position
        );
    }

    // k se indexa desde 1.
    int kth(PersistentSegmentTreeNode* currentRoot, int k) const {
        if (currentRoot == nullptr || k < 1 || k > currentRoot->count) {
            throw out_of_range("k no existe en esta version");
        }

        return kthValue(currentRoot, MIN_VALUE, MAX_VALUE, k);
    }
};

int main() {
    DynamicPersistentMultiset multiset;

    multiset.roots.push_back(
        multiset.update(multiset.roots[0], 500000000, +1)
    );
    multiset.roots.push_back(
        multiset.update(multiset.roots[1], 7, +1)
    );

    // Version 3 se ramifica desde la version 1.
    multiset.roots.push_back(
        multiset.update(multiset.roots[1], 1000000000, +1)
    );

    // Version sin modificacion.
    multiset.roots.push_back(multiset.roots[2]); // COMPARTIDO

    cout << multiset.kth(multiset.roots[2], 1) << '\n'; // 7
    cout << multiset.kth(multiset.roots[2], 2) << '\n'; // 500000000
    cout << multiset.kth(multiset.roots[3], 2) << '\n'; // 1000000000

    return 0;
}

/*
No hay build: solamente se crean los nodos visitados por actualizaciones.

Complejidad para universo U = 1e9 + 1:
- update:    O(log U) tiempo y O(log U) nodos nuevos (aprox. 31).
- frequency: O(log U) tiempo.
- kth:       O(log U) tiempo.
- root[i] = root[v]: O(1) tiempo y ningun nodo nuevo.
*/
