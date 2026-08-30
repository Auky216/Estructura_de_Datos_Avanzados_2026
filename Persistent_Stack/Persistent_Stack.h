#include "iostream"
#include "vector"

using namespace std;

struct Node{
    int value;
    Node* next;

    Node(int _value){
        this->value = _value;
        this->next = nullptr;
    }
};

class Persistent_Stack{
private:
    vector<Node*> versions;


public:

    Persistent_Stack(){
        versions.push_back(nullptr);
    }

    void push(int _value){
        Node* before = versions.back();
        Node* newNode = new Node(_value);

        newNode->next = before;
        versions.push_back(newNode);
        
    }

    void pop(){
        Node* before = versions.back();
        
        if( before == nullptr){
            return;
        }

        Node* newNode = before->next;
        versions.push_back(newNode);
    }

    int top(){
        Node* before = versions.back();

        if(before == nullptr){
            return -1;
        }

        return before->value;
    }

    int top_version(){
        return versions.size() - 1;
    }

};