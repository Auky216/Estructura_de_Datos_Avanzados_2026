#include "iostream"
#include <vector>

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

class SegmentTree{
private:
    Node* root;
    int n;

private:

    Node* build(vector<int>& A, int l, int r){
        Node* node = new Node();

        if (l == r){
            node->value = A[l];
            return node;
        }

        int mid = (l + r) / 2;

        node->left = build(A, l, mid);
        node->right = build(A, mid + 1, r);

        node->value = node->left->value + node->right->value;

        return node;

    }

    int query(Node* node,int l,int r,int ql,int qr){
        if (ql <= l && r <= qr){
            return node->value;
        }

        if (ql > r || qr < l){
            return 0;
        }

        int mid = (l + r) / 2;

        return query(node->left,l,mid,ql,qr) + query(node->right,mid+1,r,ql,qr);
    }

    void update(Node* node, int l, int r, int pos, int value){

        if(l == r){
            node->value = value;
            return;
        }

        int mid = (l + r) / 2;

        if(pos <= mid){
            update(node->left, l, mid, pos, value);
        }else{
            update(node->right, mid + 1, r, pos, value);
        }

        node->value = node->left->value + node->right->value;
    }
    

public: 

    SegmentTree(vector<int>& A){
        root = build(A,0,A.size()-1);
        this->n = A.size();
    }

    int query(int l,int r){
        return this->query(root,0,n-1,l,r);
    }

    void update(int pos, int value){
        update(root, 0, n - 1, pos, value);
    }

    




    

};
