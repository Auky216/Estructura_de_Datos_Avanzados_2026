#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

struct PersistentSegmentTreeNode {
    int count;
    int l, r;
    PersistentSegmentTreeNode* left;
    PersistentSegmentTreeNode* right;
};

class PersistentMultisetCompressed {
private:
    vector<int> coordinates;

    PersistentSegmentTreeNode* build(int l, int r) {
        PersistentSegmentTreeNode* node =
            new PersistentSegmentTreeNode{0, l, r, nullptr, nullptr};

        if (l == r) {
            return node;
        }

        int mid = (l + r) / 2;
        node->left = build(l, mid);
        node->right = build(mid + 1, r);
        return node;
    }

    PersistentSegmentTreeNode* updatePosition(
        PersistentSegmentTreeNode* previous,
        int position,
        int delta
    ) {
        PersistentSegmentTreeNode* current =
            new PersistentSegmentTreeNode(*previous); // NUEVO NODO

        current->left = previous->left;   // COMPARTIDO
        current->right = previous->right; // COMPARTIDO
        current->count = previous->count + delta;

        if (current->l == current->r) {
            return current;
        }

        int mid = (current->l + current->r) / 2;
        if (position <= mid) {
            current->left = updatePosition(previous->left, position, delta);
            current->right = previous->right; // COMPARTIDO
        } else {
            current->left = previous->left; // COMPARTIDO
            current->right = updatePosition(previous->right, position, delta);
        }

        return current;
    }

    int frequencyAt(
        PersistentSegmentTreeNode* current,
        int position
    ) const {
        if (current->l == current->r) {
            return current->count;
        }

        int mid = (current->l + current->r) / 2;
        if (position <= mid) {
            return frequencyAt(current->left, position);
        }
        return frequencyAt(current->right, position);
    }

    int kthPosition(PersistentSegmentTreeNode* current, int k) const {
        if (current->l == current->r) {
            return current->l;
        }

        int leftCount = current->left->count;
        if (leftCount >= k) {
            return kthPosition(current->left, k);
        }
        return kthPosition(current->right, k - leftCount);
    }

    int compressedPosition(int value) const {
        auto it = lower_bound(coordinates.begin(), coordinates.end(), value);
        if (it == coordinates.end() || *it != value) {
            throw invalid_argument("El valor no esta en la compresion");
        }
        return static_cast<int>(it - coordinates.begin());
    }

public:
    vector<PersistentSegmentTreeNode*> roots;

    explicit PersistentMultisetCompressed(vector<int> possibleValues) {
        sort(possibleValues.begin(), possibleValues.end());
        possibleValues.erase(
            unique(possibleValues.begin(), possibleValues.end()),
            possibleValues.end()
        );

        if (possibleValues.empty()) {
            throw invalid_argument("La compresion no puede estar vacia");
        }

        coordinates = possibleValues;
        roots.push_back(build(0, static_cast<int>(coordinates.size()) - 1));
    }

    // delta = +1 inserta y delta = -1 elimina una aparicion.
    PersistentSegmentTreeNode* update(
        PersistentSegmentTreeNode* previousRoot,
        int value,
        int delta
    ) {
        int position = compressedPosition(value);

        if (previousRoot->count + delta < 0 ||
            frequencyAt(previousRoot, position) + delta < 0) {
            throw invalid_argument("No se puede eliminar un valor ausente");
        }

        return updatePosition(previousRoot, position, delta);
    }

    int frequency(PersistentSegmentTreeNode* currentRoot, int value) const {
        return frequencyAt(currentRoot, compressedPosition(value));
    }

    // k se indexa desde 1. Devuelve el valor original, no su indice comprimido.
    int kth(PersistentSegmentTreeNode* currentRoot, int k) const {
        if (k < 1 || k > currentRoot->count) {
            throw out_of_range("k no existe en esta version");
        }

        return coordinates[kthPosition(currentRoot, k)];
    }
};

int main() {
    // Deben conocerse offline todos los valores que alguna operacion insertara.
    PersistentMultisetCompressed multiset({10, 20, 30, 40});

    // roots[0] es el multiconjunto vacio.
    multiset.roots.push_back(
        multiset.update(multiset.roots[0], 20, +1)
    ); // version 1: {20}

    multiset.roots.push_back(
        multiset.update(multiset.roots[1], 10, +1)
    ); // version 2: {10, 20}

    // Ramificacion desde una version historica arbitraria.
    multiset.roots.push_back(
        multiset.update(multiset.roots[1], 30, +1)
    ); // version 3: {20, 30}

    multiset.roots.push_back(
        multiset.update(multiset.roots[2], 20, -1)
    ); // version 4: {10}

    multiset.roots.push_back(multiset.roots[3]); // COMPARTIDO: sin cambios.

    cout << multiset.kth(multiset.roots[2], 2) << '\n'; // 20
    cout << multiset.kth(multiset.roots[3], 2) << '\n'; // 30
    cout << multiset.kth(multiset.roots[4], 1) << '\n'; // 10

    return 0;
}

/*
root[i] puede crearse desde cualquier root[v] mediante Path Copying.

Complejidad, con m valores comprimidos:
- build:     O(m) tiempo y espacio.
- update:    O(log m) tiempo y O(log m) nodos nuevos.
- frequency: O(log m) tiempo.
- kth:       O(log m) tiempo.
- root[i] = root[v]: O(1) tiempo y ningun nodo nuevo.
*/
