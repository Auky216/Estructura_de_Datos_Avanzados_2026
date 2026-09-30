#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

struct PersistentSegmentTreeNode {
    int value; // Cantidad de bits encendidos en [l, r].
    int l, r;
    PersistentSegmentTreeNode* left;
    PersistentSegmentTreeNode* right;
    int lazy; // -1: sin asignacion pendiente, 0 o 1: todo el segmento vale eso.
};

class PersistentBitSegmentTree {
private:
    int maxBit;

    void checkVersion(int version) const {
        if (version < 0 || version >= static_cast<int>(roots.size())) {
            throw out_of_range("Version del entero binario invalida");
        }
    }

    void checkPosition(int position) const {
        if (position < 0 || position > maxBit) {
            throw out_of_range("Posicion de bit invalida");
        }
    }

    PersistentSegmentTreeNode* createUniformNode(int l, int r, int bit) {
        int ones = bit * (r - l + 1);
        return new PersistentSegmentTreeNode{
            ones, l, r, nullptr, nullptr, bit
        };  // NUEVO NODO
    }

    PersistentSegmentTreeNode* cloneNode(PersistentSegmentTreeNode* previous) {
        PersistentSegmentTreeNode* current =
            new PersistentSegmentTreeNode(*previous);  // NUEVO NODO
        current->left = previous->left;    // COMPARTIDO
        current->right = previous->right;  // COMPARTIDO
        return current;
    }

    void apply(PersistentSegmentTreeNode* node, int bit) {
        node->value = bit * (node->r - node->l + 1);
        node->lazy = bit;
    }

    // Propaga sobre copias. Nunca modifica los hijos de una version antigua.
    void push(PersistentSegmentTreeNode* node) {
        if (node->l == node->r || node->lazy == -1) {
            return;
        }

        int mid = (node->l + node->r) / 2;

        if (node->left == nullptr) {
            node->left = createUniformNode(node->l, mid, node->lazy);
        } else {
            node->left = cloneNode(node->left); // NUEVO NODO por Path Copying
            apply(node->left, node->lazy);
        }

        if (node->right == nullptr) {
            node->right = createUniformNode(mid + 1, node->r, node->lazy);
        } else {
            node->right = cloneNode(node->right); // NUEVO NODO por Path Copying
            apply(node->right, node->lazy);
        }

        node->lazy = -1;
    }

    PersistentSegmentTreeNode* assignRange(
        PersistentSegmentTreeNode* previous,
        int ql,
        int qr,
        int bit
    ) {
        if (qr < previous->l || previous->r < ql) {
            return previous; // COMPARTIDO
        }

        PersistentSegmentTreeNode* current = cloneNode(previous);

        if (ql <= current->l && current->r <= qr) {
            apply(current, bit);
            return current;
        }

        push(current);
        current->left = assignRange(current->left, ql, qr, bit);
        current->right = assignRange(current->right, ql, qr, bit);
        current->value = current->left->value + current->right->value;
        return current;
    }

    int getBit(PersistentSegmentTreeNode* node, int position) const {
        if (node->lazy != -1 || node->l == node->r) {
            return node->lazy == -1 ? node->value : node->lazy;
        }

        int mid = (node->l + node->r) / 2;
        if (position <= mid) {
            return getBit(node->left, position);
        }
        return getBit(node->right, position);
    }

    int findNextZero(PersistentSegmentTreeNode* node, int position) const {
        if (node->r < position || node->value == node->r - node->l + 1) {
            return -1;
        }

        if (node->lazy == 0) {
            return max(position, node->l);
        }

        if (node->l == node->r) {
            return node->l;
        }

        int answer = findNextZero(node->left, position);
        if (answer != -1) {
            return answer;
        }
        return findNextZero(node->right, position);
    }

public:
    vector<PersistentSegmentTreeNode*> roots;

    explicit PersistentBitSegmentTree(int numberOfBits)
        : maxBit(numberOfBits - 1) {
        if (numberOfBits <= 0) {
            throw invalid_argument("La cantidad de bits debe ser positiva");
        }
        // roots[0] es el numero cero: todos sus bits estan apagados.
        roots.push_back(createUniformNode(0, maxBit, 0));
    }

    int getBit(int version, int position) const {
        checkVersion(version);
        checkPosition(position);
        return getBit(roots[version], position);
    }

    int setBit(int version, int position, int bit) {
        checkVersion(version);
        checkPosition(position);
        if (bit != 0 && bit != 1) {
            throw invalid_argument("El bit debe ser 0 o 1");
        }
        PersistentSegmentTreeNode* newRoot;

        if (getBit(version, position) == bit) {
            newRoot = roots[version]; // COMPARTIDO: no habia nada que cambiar.
        } else {
            newRoot = assignRange(roots[version], position, position, bit);
        }

        roots.push_back(newRoot);
        return static_cast<int>(roots.size()) - 1;
    }

    int findNextZero(int version, int position) const {
        checkVersion(version);
        checkPosition(position);
        return findNextZero(roots[version], position);
    }

    // Crea una version que representa roots[version] + 2^x.
    // Devuelve -1 si el carry excede la capacidad reservada.
    int addPowerOfTwo(int version, int x) {
        checkVersion(version);
        checkPosition(x);
        int nextZero = findNextZero(version, x);
        if (nextZero == -1) {
            return -1;
        }

        PersistentSegmentTreeNode* newRoot = roots[version]; // COMPARTIDO al inicio.

        // Los unos consecutivos producen carry y pasan a cero.
        if (x < nextZero) {
            newRoot = assignRange(newRoot, x, nextZero - 1, 0);
        }

        newRoot = assignRange(newRoot, nextZero, nextZero, 1);
        roots.push_back(newRoot);
        return static_cast<int>(roots.size()) - 1;
    }
};

int main() {
    PersistentBitSegmentTree number(16);

    // Construimos 7 = ...0111 partiendo de la version 0.
    int version1 = number.setBit(0, 0, 1);
    int version2 = number.setBit(version1, 1, 1);
    int version3 = number.setBit(version2, 2, 1);

    // 7 + 2^0 = 8: ...0111 -> ...1000.
    int version4 = number.addPowerOfTwo(version3, 0);

    // roots[i] representa el entero binario completo de la version i.
    for (int bit = 3; bit >= 0; --bit) {
        cout << number.getBit(version4, bit);
    }
    cout << '\n'; // 1000

    // La version anterior permanece intacta.
    for (int bit = 3; bit >= 0; --bit) {
        cout << number.getBit(version3, bit);
    }
    cout << '\n'; // 0111

    return 0;
}

/*
La posicion 0 es el bit menos significativo.

Complejidad para B bits:
- getBit:        O(log B) tiempo y O(1) nodos nuevos.
- setBit:        O(log B) tiempo y O(log B) nodos nuevos.
- findNextZero:  O(log B) tiempo gracias al contador de unos.
- addPowerOfTwo: O(log B) tiempo y O(log B) nodos nuevos.
*/
