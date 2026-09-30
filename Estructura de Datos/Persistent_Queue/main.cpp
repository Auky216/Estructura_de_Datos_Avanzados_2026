#include <iostream>
#include <vector>

using namespace std;

struct Node {
    int value;
    Node* next;

    Node(int value, Node* next) : value(value), next(next) {}
};

class PersistentStack {
public:
    vector<Node*> roots;

    PersistentStack() {
        roots.push_back(nullptr);
    }

    int push(int version, int value) {
        Node* newTop = new Node(value, roots[version]); // NUEVO NODO
        // El resto de la pila pertenece a la version anterior. // COMPARTIDO
        roots.push_back(newTop);
        return static_cast<int>(roots.size()) - 1;
    }

    int pop(int version) {
        Node* newTop = roots[version] == nullptr
            ? nullptr
            : roots[version]->next; // COMPARTIDO
        roots.push_back(newTop);
        return static_cast<int>(roots.size()) - 1;
    }

    int top(int version) const {
        return roots[version]->value;
    }

    bool empty(int version) const {
        return roots[version] == nullptr;
    }

    // Se usa cuando outStack esta vacia.
    int bottom(int version) const {
        Node* current = roots[version];
        while (current->next != nullptr) {
            current = current->next;
        }
        return current->value;
    }
};

struct QueueVersion {
    int inVersion;
    int outVersion;
    int size;
};

class PersistentQueue {
private:
    PersistentStack inStack;
    PersistentStack outStack;

    QueueVersion moveInToOut(QueueVersion state) {
        int currentIn = state.inVersion;
        int currentOut = state.outVersion;

        while (!inStack.empty(currentIn)) {
            currentOut = outStack.push(currentOut, inStack.top(currentIn));
            currentIn = inStack.pop(currentIn);
        }

        state.inVersion = currentIn;
        state.outVersion = currentOut;
        return state;
    }

public:
    // roots[i] guarda las dos pilas que forman la cola en la version i.
    vector<QueueVersion> roots;

    PersistentQueue() {
        roots.push_back({0, 0, 0});
    }

    int push(int version, int value) {
        QueueVersion previous = roots[version];
        int newInVersion = inStack.push(previous.inVersion, value);

        roots.push_back({
            newInVersion,
            previous.outVersion, // COMPARTIDO
            previous.size + 1
        });
        return static_cast<int>(roots.size()) - 1;
    }

    int front(int version) const {
        const QueueVersion& current = roots[version];
        if (current.size == 0) {
            return -1;
        }

        if (!outStack.empty(current.outVersion)) {
            return outStack.top(current.outVersion);
        }
        return inStack.bottom(current.inVersion);
    }

    int pop(int version) {
        QueueVersion current = roots[version];

        if (current.size == 0) {
            roots.push_back(current); // COMPARTIDO
            return static_cast<int>(roots.size()) - 1;
        }

        if (outStack.empty(current.outVersion)) {
            current = moveInToOut(current);
        }

        current.outVersion = outStack.pop(current.outVersion);
        --current.size;
        roots.push_back(current);
        return static_cast<int>(roots.size()) - 1;
    }

    bool empty(int version) const {
        return roots[version].size == 0;
    }
};

int main() {
    PersistentQueue queue;

    int version1 = queue.push(0, 10);
    int version2 = queue.push(version1, 20);
    int version3 = queue.pop(version2);
    int version4 = queue.push(version1, 99); // Ramificacion desde version1.

    cout << queue.front(version2) << '\n'; // 10
    cout << queue.front(version3) << '\n'; // 20
    cout << queue.front(version4) << '\n'; // 10

    return 0;
}

/*
Complejidad:
- push:  O(1) tiempo y espacio nuevo.
- front: O(1) si outStack no esta vacia; O(n) si debe mirar el fondo de inStack.
- pop:   O(1) normalmente y O(n) cuando transfiere inStack hacia outStack.

ADVERTENCIA DE PERSISTENCIA TOTAL:
En una historia lineal, la transferencia permite el analisis amortizado habitual.
Si muchas versiones se ramifican desde un mismo estado antiguo con outStack vacia,
la misma transferencia puede repetirse. Por eso no se garantiza O(1) amortizado
global bajo persistencia total. Esta es intencionalmente la plantilla basica.
*/
