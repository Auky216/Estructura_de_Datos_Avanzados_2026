#pragma once

#include <vector>

struct Node {
    long long key;
    int id;
    Node* child;
    Node* sibling;
    Node* prev;

    Node(int nodeId, long long nodeKey) {
        this->id = nodeId;
        this->key = nodeKey;
        this->child = nullptr;
        this->sibling = nullptr;
        this->prev = nullptr;
    }
};

class PairingHeap {
private:
    Node* root;
    std::vector<Node*> nodeOf;

    // Una clave menor tiene prioridad. Si empatan, gana el id menor.
    bool hasHigherPriority(Node* first, Node* second) {
        if (first->key != second->key) {
            return first->key < second->key;
        }

        return first->id < second->id;
    }

    // Une dos arboles. La raiz con mayor prioridad se mantiene como raiz.
    Node* mergeTrees(Node* first, Node* second) {
        if (first == nullptr) {
            return second;
        }
        if (second == nullptr) {
            return first;
        }

        Node* winner;
        Node* loser;

        if (hasHigherPriority(first, second)) {
            winner = first;
            loser = second;
        } else {
            winner = second;
            loser = first;
        }

        // El arbol que pierde se convierte en el primer hijo del ganador.
        loser->sibling = winner->child;
        if (winner->child != nullptr) {
            winner->child->prev = loser;
        }

        loser->prev = winner;
        winner->child = loser;

        // Una raiz no tiene hermano ni nodo anterior.
        winner->sibling = nullptr;
        winner->prev = nullptr;

        return winner;
    }

    // Separa un nodo de la lista de hijos de su padre.
    void detachFromParent(Node* node) {
        Node* previousNode = node->prev;

        if (previousNode->child == node) {
            // El nodo era el primer hijo de su padre.
            previousNode->child = node->sibling;
        } else {
            // El nodo tenia un hermano a su izquierda.
            previousNode->sibling = node->sibling;
        }

        if (node->sibling != nullptr) {
            node->sibling->prev = previousNode;
        }

        node->sibling = nullptr;
        node->prev = nullptr;
    }

    // Combina los hijos de una raiz en dos pasadas: izquierda a derecha y
    // luego derecha a izquierda. El resultado es un nuevo pairing heap.
    Node* mergeChildrenByPairs(Node* firstChild) {
        if (firstChild == nullptr) {
            return nullptr;
        }

        std::vector<Node*> mergedPairs;
        Node* current = firstChild;

        // Primera pasada: unir hijos consecutivos por parejas.
        while (current != nullptr) {
            Node* firstTree = current;
            Node* secondTree = current->sibling;

            if (secondTree == nullptr) {
                current = nullptr;
                firstTree->sibling = nullptr;
                firstTree->prev = nullptr;
                mergedPairs.push_back(firstTree);
            } else {
                current = secondTree->sibling;

                firstTree->sibling = nullptr;
                firstTree->prev = nullptr;
                secondTree->sibling = nullptr;
                secondTree->prev = nullptr;

                Node* mergedTree = mergeTrees(firstTree, secondTree);
                mergedPairs.push_back(mergedTree);
            }
        }

        // Segunda pasada: unir los resultados desde la derecha.
        Node* result = mergedPairs.back();
        for (int index = static_cast<int>(mergedPairs.size()) - 2;
             index >= 0;
             index--) {
            result = mergeTrees(mergedPairs[index], result);
        }

        return result;
    }

public:
    PairingHeap(int maximumId) {
        this->root = nullptr;
        this->nodeOf.assign(maximumId + 1, nullptr);
    }

    void insert(int id, long long key) {
        Node* newNode = new Node(id, key);
        this->nodeOf[id] = newNode;
        this->root = mergeTrees(this->root, newNode);
    }

    int extractMin() {
        Node* oldRoot = this->root;
        int minimumId = oldRoot->id;

        this->root = mergeChildrenByPairs(oldRoot->child);
        this->nodeOf[minimumId] = nullptr;

        delete oldRoot;
        return minimumId;
    }

    void decreaseKey(int id, long long newKey) {
        Node* node = this->nodeOf[id];
        node->key = newKey;

        if (node == this->root) {
            return;
        }

        detachFromParent(node);
        this->root = mergeTrees(this->root, node);
    }

    bool empty() {
        return this->root == nullptr;
    }
};
