#include "iostream"
#include "vector"
#include "string"

using namespace std;

struct Node{
    int value;
    Node* left;
    Node* right;

    Node(){
        this->value = 0;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class PersisentArray{
private:
vector<Node*> versions;
int n;

private:

Node* build(int* array,int l,int r){

    Node* node = new Node();

    if(l == r){
        node->value = array[l];
        return node;
    }

    int mid = (l + r) / 2;

    node->left = build(array,l,mid);
    node->right = build(array,mid+1,r);

    return node;
}

Node* create(Node* node,int l,int r,int j,int x){

    Node* newNode = new Node();

    newNode->value = node->value;
    newNode->left = node->left;
    newNode->right = node->right;

    if(l == r){
        newNode->value = x;
        return newNode;
    }

    int mid = (l + r) / 2;

    if(j <= mid){
        newNode->left = create(node->left,l,mid,j,x);
    }else{
        newNode->right = create(node->right,mid+1,r,j,x);
    }

    return newNode;
}

int get(Node* node,int l,int r,int j){

    if(l == r){
        return node->value;
    }

    int mid = (l + r) / 2;

    if(j <= mid){
        return get(node->left,l,mid,j);
    }else{
        return get(node->right,mid+1,r,j);
    }
}

public:

PersisentArray(int* array,int _n){
    
    this->n = _n;

    Node* root = build(array,0,n-1);

    this->versions.push_back(root);
    
}



void create(int i, int j, int x){

    Node* newVersion = create(
        versions[i-1],
        0,
        n-1,
        j-1,
        x
    );

    versions.push_back(newVersion);
}

int get(int i,int j){

    return get(
        versions[i-1],
        0,
        n-1,
        j-1
    );
}

};

int main(){

int n;
int instruction;

cin >> n;

int* array = new int[n];

for(int i = 0; i < n; i++){
    cin >> array[i];
}

PersisentArray p1(array, n);

cin >> instruction;

for(int i = 0; i < instruction; i++){

    string inst;
    int version;
    int key;
    int value;

    cin >> inst;

    if(inst == "create"){
        cin >> version >> key >> value;

        p1.create(version, key, value);
    }
    else if(inst == "get"){
        cin >> version >> key;

        cout << p1.get(version, key) << endl;
    }
}

return 0;

}