#include <iostream>
#include <vector>

using namespace std;

struct PersistentSegmentTreeNode {
    int value;
    int l, r;
    PersistentSegmentTreeNode* left;
    PersistentSegmentTreeNode* right;
};

class PersistentSegmentTreeByPrefix {
private:
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

    int queryOne(PersistentSegmentTreeNode* node, int ql, int qr) const {
        if (qr < node->l || node->r < ql) {
            return 0;
        }
        if (ql <= node->l && node->r <= qr) {
            return node->value;
        }

        return queryOne(node->left, ql, qr) +
               queryOne(node->right, ql, qr);
    }

public:
    vector<PersistentSegmentTreeNode*> roots;

    PersistentSegmentTreeByPrefix(int minPosition, int maxPosition) {
        // roots[0] representa el prefijo vacio.
        roots.push_back(build(minPosition, maxPosition));
    }

    // Suma delta en position y devuelve la raiz nueva.
    PersistentSegmentTreeNode* update(
        PersistentSegmentTreeNode* prevRoot,
        int position,
        int delta
    ) {
        PersistentSegmentTreeNode* current =
            new PersistentSegmentTreeNode(*prevRoot);  // NUEVO NODO

        current->left = prevRoot->left;    // COMPARTIDO
        current->right = prevRoot->right;  // COMPARTIDO

        if (current->l == current->r) {
            current->value = prevRoot->value + delta;
            return current;
        }

        int mid = (current->l + current->r) / 2;
        if (position <= mid) {
            current->left = update(prevRoot->left, position, delta);
            current->right = prevRoot->right;  // COMPARTIDO
        } else {
            current->left = prevRoot->left;  // COMPARTIDO
            current->right = update(prevRoot->right, position, delta);
        }

        current->value = current->left->value + current->right->value;
        return current;
    }

    // Agrega un elemento al siguiente prefijo.
    int append(int position, int delta = 1) {
        PersistentSegmentTreeNode* newRoot =
            update(roots.back(), position, delta);
        roots.push_back(newRoot);
        return static_cast<int>(roots.size()) - 1;
    }

    // Consulta la diferencia rootR - rootL dentro de [ql, qr].
    int query(
        PersistentSegmentTreeNode* rootR,
        PersistentSegmentTreeNode* rootL,
        int ql,
        int qr
    ) const {
        return queryOne(rootR, ql, qr) - queryOne(rootL, ql, qr);
    }

    // Consulta los elementos originales con indices [l, r], indexados desde 1.
    int query(int l, int r, int ql, int qr) const {
        return query(roots[r], roots[l - 1], ql, qr);
    }
};

int main() {
    // Las posiciones suelen ser valores comprimidos o frecuencias.
    vector<int> values = {2, 0, 2, 1};
    PersistentSegmentTreeByPrefix tree(0, 3);

    for (int value : values) {
        tree.append(value);
    }

    // roots[i] contiene las frecuencias de los primeros i elementos.
    // En los elementos [2, 4] hay dos valores dentro del rango [0, 1].
    cout << tree.query(2, 4, 0, 1) << '\n';

    return 0;
}

/*
Complejidad:
- build:  O(m), donde m es la cantidad de posiciones posibles.
- append/update: O(log m) tiempo y O(log m) nodos nuevos.
- query: O(log m) para un rango de posiciones.
- roots[r] - roots[l - 1] aisla los elementos originales del intervalo [l, r].
*/
