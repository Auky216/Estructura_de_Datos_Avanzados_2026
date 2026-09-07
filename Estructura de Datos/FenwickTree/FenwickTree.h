#ifndef FENWICKTREE_H
#define FENWICKTREE_H

#include <vector>

using namespace std;

class FenwickTree{
private:
    vector<int> tree;
    int n;

    int lowbit(int i){
        return i & -i;
    }

public:

    FenwickTree(int _n){
        this->n = _n;
        this->tree.resize(n + 1, 0);
    }

    void update(int i, int value){

        while(i <= n){
            tree[i] += value;
            i = i + lowbit(i);
        }
    }

    int query(int i){

        int sum = 0;

        while(i > 0){
            sum += tree[i];
            i = i - lowbit(i);
        }

        return sum;
    }

    int query(int a, int b){
        return query(b) - query(a - 1);
    }
};

#endif