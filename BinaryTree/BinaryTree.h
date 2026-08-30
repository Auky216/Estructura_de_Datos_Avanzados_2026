#include "iostream"

using namespace std;

struct Node{
    int value;
    Node* left;
    Node* right;

    Node(int _value){
        this->value = _value;
        this->left = nullptr;
        this->right = nullptr;
    }
};



class BinaryTree{
private:
    Node* head;

private:

    Node* remove(Node* root, int value){
    if (root == nullptr) return nullptr;

    if (value < root->value){
        root->left = remove(root->left, value);
    }
    else if (value > root->value){
        root->right = remove(root->right, value);
    }
    else{
        // Caso 1: sin hijo izquierdo
        if (root->left == nullptr){
            Node* temp = root->right;
            delete root;
            return temp;
        }

        // Caso 2: sin hijo derecho
        if (root->right == nullptr){
            Node* temp = root->left;
            delete root;
            return temp;
        }

        // Caso 3: tiene dos hijos
        Node* temp = root->right;

        while (temp->left != nullptr){
            temp = temp->left;
        }

        root->value = temp->value;
        root->right = remove(root->right, temp->value);
    }

    return root;
}

public:
    BinaryTree(){
        this->head = nullptr;
    }

    void insert(int _value){

        if (head == nullptr){
            head = new Node(_value);
            return;
        }

        Node* temp = head;

        while (true){
            if (_value < temp->value){
                if (temp->left == nullptr){
                    temp->left = new Node(_value);
                    return;
                }
                temp = temp->left;
            } else {
                if (temp->right == nullptr){
                    temp->right = new Node(_value);
                    return;
                }
                temp = temp->right;
            }
        }
    }

    void remove(int value){
        head = remove(head, value);
    }
    
};