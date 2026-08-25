#include <iostream>
#include <vector>
using namespace std;

struct Node {
    long long key;
    int id;
    Node *child;
    Node *sibling;
    Node *prev;

    Node(int id_, long long key_) : key(key_), id(id_), child(nullptr), sibling(nullptr), prev(nullptr) {}
};

class PairingHeap {
private:
    Node* root;
    vector<Node*> nodeOf;

    bool hasHigherPriority(Node* a, Node* b) {
        if (a->key != b->key) return a->key < b->key;
        return a->id < b->id;
    }

    Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;

        Node* winner = hasHigherPriority(a, b) ? a : b;
        Node* loser = (winner == a) ? b : a;

        loser->sibling = winner->child;
        if (winner->child) winner->child->prev = loser;
        loser->prev = winner;
        winner->child = loser;

        winner->sibling = nullptr;
        winner->prev = nullptr;

        return winner;
    }

    void detach(Node* node) {
        if (node->prev->child == node) {
            node->prev->child = node->sibling;
        } else {
            node->prev->sibling = node->sibling;
        }
        if (node->sibling) node->sibling->prev = node->prev;

        node->sibling = nullptr;
        node->prev = nullptr;
    }

    Node* mergePairs(Node* first) {
        if (!first || !first->sibling) return first;

        vector<Node*> merged;
        Node* cur = first;
        while (cur) {
            Node* a = cur;
            Node* b = cur->sibling;
            if (b) {
                cur = b->sibling;
                a->sibling = a->prev = nullptr;
                b->sibling = b->prev = nullptr;
                merged.push_back(merge(a, b));
            } else {
                a->sibling = a->prev = nullptr;
                merged.push_back(a);
                cur = nullptr;
            }
        }

        Node* result = merged.back();
        for (int i = (int)merged.size() - 2; i >= 0; i--) {
            result = merge(merged[i], result);
        }
        return result;
    }

public:
    PairingHeap(int maxId) : root(nullptr) {
        nodeOf.assign(maxId + 5, nullptr);
    }

    void insert(int id, long long key) {
        Node* node = new Node(id, key);
        nodeOf[id] = node;
        root = merge(root, node);
    }

    int extractMin() {
        Node* oldRoot = root;
        int ans = oldRoot->id;

        root = mergePairs(oldRoot->child);

        nodeOf[ans] = nullptr;
        delete oldRoot;

        return ans;
    }

    void decreaseKey(int id, long long newKey) {
        Node* node = nodeOf[id];
        node->key = newKey;

        if (node == root) return;

        detach(node);
        root = merge(root, node);
    }

    bool empty() {
        return root == nullptr;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin>>n>>q;

    PairingHeap heap(n + q + 5);

    for (int i = 1; i <= n; i++) {
        long long a;
        cin >> a;
        heap.insert(i, a);
    }

    int nextId = n + 1;
    string output;

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            long long v;
            cin >> v;
            heap.insert(nextId, v);
            nextId++;
        } else if (type == 2) {
            output += to_string(heap.extractMin()) + '\n';
        } else {
            int id;
            long long v;
            cin >> id >> v;
            heap.decreaseKey(id, v);
        }
    }

    cout << output;

    return 0;
}
