#include <iostream>

using namespace std;

struct Node{
    int value;
    Node* next;

    Node(int _value){
        this->value = _value;
        this->next = nullptr;
    }
};

class Stack{
private:
    Node* head;

public:

    Stack(){
        this->head = nullptr;
    }

    void push(int _value){
        Node* nuevo = new Node(_value);

        nuevo->next = head;
        head = nuevo;
    }

    void pop(){
        if (head == nullptr){
            return;
        }

        Node* temp = head;
        head = head->next;

        delete temp;
    }

    int top(){
        if (head == nullptr){
            return -1;
        }

        return head->value;
    }
};