#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

const int ALPHABET = 26;
const int MAXQ = 200000;

// Debe ser mayor que 1 + la suma de longitudes de todas las inserciones.
const int NODES = 1000005;

struct Node {
    int next[ALPHABET];
    int cnt;
};

int nodes = 0;
Node trie[NODES];
int root[MAXQ + 5];

// El nodo 0 es nulo y esta inicializado con ceros globalmente.
int clone_node(int old) {
    if (nodes + 1 >= NODES) {
        throw runtime_error("Se agoto el pool de nodos");
    }

    int current = ++nodes; // NUEVO NODO
    trie[current] = trie[old];
    // Los 26 hijos copiados siguen compartidos hasta modificar uno. // COMPARTIDO
    return current;
}

// Devuelve una raiz nueva. previousRoot nunca se modifica.
int insert(int previousRoot, const string& word) {
    int newRoot = clone_node(previousRoot);
    int previous = previousRoot;
    int current = newRoot;

    // La raiz cuenta todas las palabras, util para el prefijo vacio.
    trie[current].cnt = trie[previous].cnt + 1;

    for (char letter : word) {
        if (letter < 'a' || letter > 'z') {
            throw invalid_argument("El trie solo acepta letras a-z");
        }

        int c = letter - 'a';
        int previousChild = trie[previous].next[c];
        int newChild = clone_node(previousChild);

        // Los otros 25 hijos de current permanecen compartidos.
        trie[current].next[c] = newChild;

        previous = previousChild;
        current = newChild;
        trie[current].cnt = trie[previous].cnt + 1;
    }

    return newRoot;
}

// Cantidad de palabras de esta version que empiezan con prefix.
int prefixCount(int currentRoot, const string& prefix) {
    int current = currentRoot;

    for (char letter : prefix) {
        if (letter < 'a' || letter > 'z') {
            return 0;
        }

        current = trie[current].next[letter - 'a'];
        if (current == 0) {
            return 0;
        }
    }

    return trie[current].cnt;
}

// rootL normalmente es root[l - 1] y rootR normalmente es root[r].
int prefixCountRange(int rootL, int rootR, const string& prefix) {
    return prefixCount(rootR, prefix) - prefixCount(rootL, prefix);
}

int main() {
    root[0] = 0; // Diccionario vacio.

    // CASO 1: versiones por prefijo.
    root[1] = insert(root[0], "casa");
    root[2] = insert(root[1], "cama");
    root[3] = insert(root[2], "carro");

    cout << prefixCount(root[3], "ca") << '\n'; // 3

    // Entre las palabras s2 ... s3, dos empiezan con "ca".
    cout << prefixCountRange(root[1], root[3], "ca") << '\n'; // 2

    // CASO 2: partir de una version historica arbitraria.
    root[4] = insert(root[1], "perro");
    cout << prefixCount(root[4], "ca") << '\n'; // 1

    // Operacion sin modificacion: volver o copiar una version.
    root[5] = root[2]; // COMPARTIDO
    cout << prefixCount(root[5], "ca") << '\n'; // 2

    return 0;
}

/*
root[i] puede representar:
- Las primeras i palabras: root[i] = insert(root[i - 1], palabra).
- Una rama historica:      root[i] = insert(root[t], palabra).
- Un regreso sin cambios:  root[i] = root[t].

Complejidad:
- insert: O(|word|) tiempo y O(|word| + 1) nodos nuevos.
- prefixCount: O(|prefix|) tiempo.
- prefixCountRange: O(|prefix|) tiempo.
- root[i] = root[t]: O(1) tiempo y ningun nodo nuevo.
*/
