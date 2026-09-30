#ifndef PERSISTENT_STACK_H
#define PERSISTENT_STACK_H

#include <vector>

using namespace std;

struct Node {
    int value;
    Node* next;

    Node(int value, Node* next) : value(value), next(next) {}
};

class PersistentStack {
public:
    // roots[i] apunta al tope de la pila en la version i.
    vector<Node*> roots;

    PersistentStack() {
        roots.push_back(nullptr);
    }

    int push(int version, int value) {
        Node* previousTop = roots[version];
        Node* newTop = new Node(value, previousTop); // NUEVO NODO
        // newTop->next conserva el resto de la version anterior. // COMPARTIDO
        roots.push_back(newTop);
        return static_cast<int>(roots.size()) - 1;
    }

    int pop(int version) {
        Node* previousTop = roots[version];

        if (previousTop == nullptr) {
            roots.push_back(nullptr); // COMPARTIDO
        } else {
            roots.push_back(previousTop->next); // COMPARTIDO
        }

        return static_cast<int>(roots.size()) - 1;
    }

    int top(int version) const {
        if (roots[version] == nullptr) {
            return -1;
        }
        return roots[version]->value;
    }

    bool empty(int version) const {
        return roots[version] == nullptr;
    }

    int latestVersion() const {
        return static_cast<int>(roots.size()) - 1;
    }
};

#endif
