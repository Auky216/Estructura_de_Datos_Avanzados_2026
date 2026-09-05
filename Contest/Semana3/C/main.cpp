#include "iostream"
#include "vector"
using namespace std;

const int LOG = 20;

struct Node{
int value;
Node* next;
Node* up[LOG];

Node(int _value, Node* _next = nullptr){
    this->value = _value;
    this->next = _next;

    this->up[0] = _next;

    for(int i = 1; i < LOG; i++){

        if(this->up[i-1] != nullptr){
            this->up[i] = this->up[i-1]->up[i-1];
        }else{
            this->up[i] = nullptr;
        }
    }
}

};

class PersistentQueue{
private:
vector<Node*> versions;
vector<int> sizes;

private:

Node* getAncestor(Node* node, int distance){

    for(int i = 0; i < LOG; i++){

        if(distance & (1 << i)){
            node = node->up[i];
        }
    }

    return node;
}

public:
PersistentQueue(){
this->versions.push_back(nullptr);
this->sizes.push_back(0);
}

void push(int _versions , int _value){

   Node* oldNode = this->versions[_versions];

   Node* newNode = new Node(_value, oldNode);

   versions.push_back(newNode);

   sizes.push_back(sizes[_versions] + 1);
    
}

int pop(int _version){

    Node* oldNode = versions[_version];

    if(oldNode == nullptr){
        return -1;
    }

    int size = sizes[_version];

    Node* front = getAncestor(oldNode, size - 1);

    versions.push_back(oldNode);

    sizes.push_back(size - 1);

    return front->value;

}

void print(){

    for(int i = 0; i < versions.size(); i++){

        Node* tempVersion = versions[i];

        cout<<"version "<<i<<" : ";

        int size = sizes[i];

        vector<int> values;

        for(int j = 0; j < size; j++){
            values.push_back(tempVersion->value);
            tempVersion = tempVersion->next;
        }

        for(int j = values.size() - 1; j >= 0; j--){
            cout<<values[j]<<" -> ";
        }

        cout<<endl;
    }
}

};

int main(){

int n;
cin >> n;

PersistentQueue q;

for(int i = 1; i <= n; i++){

    int operation;
    cin >> operation;

    if(operation == 1){

        int version;
        int value;

        cin >> version >> value;

        q.push(version, value);

    }else if(operation == -1){

        int version;

        cin >> version;

        cout << q.pop(version) << endl;
    }
}

return 0;

}