#ifndef PERSISTENT_STACK_H
#define PERSISTENT_STACK_H

#include <stdexcept>
#include <vector>

using namespace std;

struct Node {
    int value;
    Node* next;

    Node(int value, Node* next) : value(value), next(next) {}
};

class PersistentStack {
private:
    void checkVersion(int version) const {
        if (version < 0 || version >= static_cast<int>(roots.size())) {
            throw out_of_range("Version de pila invalida");
        }
    }

public:
    // roots[i] apunta al tope de la pila en la version i.
    vector<Node*> roots;

    PersistentStack() {
        roots.push_back(nullptr);
    }

    int push(int version, int value) {
        checkVersion(version);
        Node* previousTop = roots[version];
        Node* newTop = new Node(value, previousTop); // NUEVO NODO
        // newTop->next conserva el resto de la version anterior. // COMPARTIDO
        roots.push_back(newTop);
        return static_cast<int>(roots.size()) - 1;
    }

    int pop(int version) {
        checkVersion(version);
        Node* previousTop = roots[version];

        if (previousTop == nullptr) {
            roots.push_back(nullptr); // COMPARTIDO
        } else {
            roots.push_back(previousTop->next); // COMPARTIDO
        }

        return static_cast<int>(roots.size()) - 1;
    }

    int top(int version) const {
        checkVersion(version);
        if (roots[version] == nullptr) {
            return -1;
        }
        return roots[version]->value;
    }

    bool empty(int version) const {
        checkVersion(version);
        return roots[version] == nullptr;
    }

    int latestVersion() const {
        return static_cast<int>(roots.size()) - 1;
    }
};

#endif
