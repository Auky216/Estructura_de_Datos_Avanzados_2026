#include <iostream>
#include <stdexcept>

using namespace std;

// Enteros no negativos de 31 bits: se recorren los bits L ... 0.
const int L = 30;
const int MAXQ = 200000;
const int NODES = 1 + (MAXQ + 5) * (L + 2);

int nodes = 0;
int root[MAXQ + 5];
int frec[NODES];
int trie[2][NODES];

// El nodo 0 es el nodo nulo y permanece lleno de ceros.
int add_node(int basis = -1) {
    if (nodes + 1 >= NODES) {
        throw runtime_error("Se agoto el pool de nodos");
    }

    int pos = ++nodes; // NUEVO NODO

    if (basis == -1) {
        frec[pos] = 0;
        trie[0][pos] = 0;
        trie[1][pos] = 0;
    } else {
        frec[pos] = frec[basis];
        trie[0][pos] = trie[0][basis]; // COMPARTIDO
        trie[1][pos] = trie[1][basis]; // COMPARTIDO
    }

    return pos;
}

// Insercion en el estilo del profesor. pos ya debe ser una copia de last.
void insert(int x, int last, int pos) {
    frec[pos] = frec[last] + 1;

    for (int bit = L; bit >= 0; --bit) {
        int direction = (x >> bit) & 1;

        trie[direction ^ 1][pos] = trie[direction ^ 1][last]; // COMPARTIDO
        trie[direction][pos] = add_node(trie[direction][last]);

        last = trie[direction][last];
        pos = trie[direction][pos];
        frec[pos] = frec[last] + 1;
    }
}

// Forma comoda: crea y devuelve la nueva raiz.
int insert(int previousRoot, int value) {
    int newRoot = add_node(previousRoot); // NUEVO NODO
    insert(value, previousRoot, newRoot);
    return newRoot;
}

// rootL y rootR son los nodos actuales de las dos versiones.
// Devuelve cuantos elementos de rootR - rootL siguen por la rama bit.
int countBranch(int rootL, int rootR, int bit) {
    int childL = trie[bit][rootL];
    int childR = trie[bit][rootR];
    return frec[childR] - frec[childL];
}

// Devuelve el mayor valor de (x XOR y) dentro de rootR - rootL.
int maximizeXor(int x, int rootL, int rootR) {
    if (frec[rootR] - frec[rootL] <= 0) {
        return -1;
    }

    int answer = 0;

    for (int bit = L; bit >= 0; --bit) {
        int xBit = (x >> bit) & 1;
        int desired = xBit ^ 1;

        if (countBranch(rootL, rootR, desired) > 0) {
            answer |= (1 << bit);
            rootL = trie[desired][rootL];
            rootR = trie[desired][rootR];
        } else {
            rootL = trie[xBit][rootL];
            rootR = trie[xBit][rootR];
        }
    }

    return answer;
}

// Devuelve el menor valor de (x XOR y) dentro de rootR - rootL.
int minimizeXor(int x, int rootL, int rootR) {
    if (frec[rootR] - frec[rootL] <= 0) {
        return -1;
    }

    int answer = 0;

    for (int bit = L; bit >= 0; --bit) {
        int xBit = (x >> bit) & 1;

        if (countBranch(rootL, rootR, xBit) > 0) {
            // Elegir el mismo bit produce un bit 0 en el XOR.
            rootL = trie[xBit][rootL];
            rootR = trie[xBit][rootR];
        } else {
            answer |= (1 << bit);
            rootL = trie[xBit ^ 1][rootL];
            rootR = trie[xBit ^ 1][rootR];
        }
    }

    return answer;
}

// k se indexa desde 1. Devuelve el k-esimo menor valor de (x XOR y).
int kthXor(int x, int k, int rootL, int rootR) {
    int total = frec[rootR] - frec[rootL];
    if (k < 1 || k > total) {
        return -1;
    }

    int answer = 0;

    for (int bit = L; bit >= 0; --bit) {
        int xBit = (x >> bit) & 1;

        // Rama xBit: el bit actual del XOR seria 0.
        int cntZero = countBranch(rootL, rootR, xBit);

        if (k <= cntZero) {
            rootL = trie[xBit][rootL];
            rootR = trie[xBit][rootR];
        } else {
            // Rama xBit ^ 1: el bit actual del XOR sera 1.
            k -= cntZero;
            answer |= (1 << bit);
            rootL = trie[xBit ^ 1][rootL];
            rootR = trie[xBit ^ 1][rootR];
        }
    }

    return answer;
}

// Cuenta cuantos valores y cumplen (x XOR y) < K en rootR - rootL.
int countXorLessThan(int x, long long K, int rootL, int rootR) {
    if (K <= 0) {
        return 0;
    }
    if (K >= (1LL << (L + 1))) {
        return frec[rootR] - frec[rootL];
    }

    int answer = 0;

    for (int bit = L; bit >= 0; --bit) {
        int xBit = (x >> bit) & 1;
        int kBit = (K >> bit) & 1LL;

        if (kBit == 1) {
            // Todos los XOR con bit 0 aqui ya son menores que K.
            answer += countBranch(rootL, rootR, xBit);

            // Para seguir empatados con K, el XOR debe tener bit 1.
            rootL = trie[xBit ^ 1][rootL];
            rootR = trie[xBit ^ 1][rootR];
        } else {
            // Para no superar K, el XOR debe tener bit 0.
            rootL = trie[xBit][rootL];
            rootR = trie[xBit][rootR];
        }

        if (rootR == 0 && rootL == 0) {
            break;
        }
    }

    return answer;
}

int main() {
    // PATRON A: versiones lineales. root[i] contiene x1 ... xi.
    root[0] = add_node();
    int values[] = {5, 1, 7, 4};

    for (int i = 1; i <= 4; ++i) {
        root[i] = insert(root[i - 1], values[i - 1]);
    }

    // En [2, 4] estan {1, 7, 4}; se usa root[4] - root[1].
    cout << maximizeXor(2, root[1], root[4]) << '\n'; // 6
    cout << minimizeXor(2, root[1], root[4]) << '\n'; // 3
    cout << kthXor(2, 2, root[1], root[4]) << '\n';  // 5
    cout << countXorLessThan(2, 6, root[1], root[4]) << '\n'; // 2

    // PATRON B: root[i] = insert(root[versionAnterior], value).
    root[5] = insert(root[1], 10);
    root[6] = root[2]; // COMPARTIDO: volver exactamente a una version anterior.

    // PATRON C: versiones en arbol.
    // root[i] = insert(root[parent[i]], value[i]);
    // Para el camino ancestro w ... descendiente u:
    // usar root[u] - root[parent[w]].

    return 0;
}

/*
Complejidad, con L = cantidad de bits:
- insert:               O(L) tiempo y O(L) nodos nuevos.
- maximize/minimize:    O(L) tiempo.
- kthXor:               O(L) tiempo.
- countXorLessThan:     O(L) tiempo.
- Asignar root[i] = root[v]: O(1) tiempo y ningun nodo nuevo.

Las consultas funcionan tanto para prefijos root[r] - root[l - 1] como para
un camino de un arbol de versiones root[u] - root[parent[w]].
*/
