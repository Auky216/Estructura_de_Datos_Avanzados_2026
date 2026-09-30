#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

struct Node {
    Node* children[26];
    bool endWord;

    Node() : endWord(false) {
        for (Node*& child : children) {
            child = nullptr;
        }
    }
};

class Trie {
private:
    Node* root;
    int numberOfWords;

    static int letterIndex(char letter) {
        if (letter < 'a' || letter > 'z') {
            throw invalid_argument("El trie solo acepta letras a-z");
        }
        return letter - 'a';
    }

    bool hasChildren(Node* node) const {
        for (Node* child : node->children) {
            if (child != nullptr) {
                return true;
            }
        }
        return false;
    }

    // Devuelve true cuando el nodo actual quedo inutil y puede eliminarse.
    bool erase(Node* node, const string& word, int position, bool& removed) {
        if (position == static_cast<int>(word.size())) {
            if (!node->endWord) {
                return false;
            }
            node->endWord = false;
            removed = true;
        } else {
            int index = letterIndex(word[position]);
            Node* child = node->children[index];
            if (child == nullptr) {
                return false;
            }

            if (erase(child, word, position + 1, removed)) {
                delete child;
                node->children[index] = nullptr;
            }
        }

        return !node->endWord && !hasChildren(node);
    }

    void clear(Node* node) {
        if (node == nullptr) {
            return;
        }
        for (Node* child : node->children) {
            clear(child);
        }
        delete node;
    }

public:
    Trie() : root(new Node()), numberOfWords(0) {}

    ~Trie() {
        clear(root);
    }

    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    // Devuelve false si la palabra ya estaba guardada.
    bool insert(const string& word) {
        // Validar antes evita dejar un camino parcial si aparece un caracter malo.
        for (char letter : word) {
            letterIndex(letter);
        }

        Node* current = root;

        for (char letter : word) {
            int index = letterIndex(letter);
            if (current->children[index] == nullptr) {
                current->children[index] = new Node();
            }
            current = current->children[index];
        }

        if (current->endWord) {
            return false;
        }

        current->endWord = true;
        ++numberOfWords;
        return true;
    }

    bool search(const string& word) const {
        Node* current = root;

        for (char letter : word) {
            int index = letterIndex(letter);
            current = current->children[index];
            if (current == nullptr) {
                return false;
            }
        }

        return current->endWord;
    }

    bool startsWith(const string& prefix) const {
        Node* current = root;

        for (char letter : prefix) {
            int index = letterIndex(letter);
            current = current->children[index];
            if (current == nullptr) {
                return false;
            }
        }

        return true;
    }

    bool erase(const string& word) {
        bool removed = false;
        erase(root, word, 0, removed);
        if (removed) {
            --numberOfWords;
        }
        return removed;
    }

    int size() const {
        return numberOfWords;
    }

    bool empty() const {
        return numberOfWords == 0;
    }
};

int main() {
    Trie trie;

    trie.insert("casa");
    trie.insert("cama");
    trie.insert("carro");

    cout << trie.search("casa") << '\n';       // 1
    cout << trie.search("cas") << '\n';        // 0
    cout << trie.startsWith("ca") << '\n';     // 1

    trie.erase("casa");
    cout << trie.search("casa") << '\n';       // 0
    cout << trie.startsWith("ca") << '\n';     // 1: quedan cama y carro

    return 0;
}

/*
Complejidad para una cadena de longitud m:
- insert, search, startsWith y erase: O(m).
- espacio total: O(cantidad de caracteres almacenados).
*/
