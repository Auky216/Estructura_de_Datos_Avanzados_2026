#include <iostream>
#include <vector>
#include "SegmentTree.h"

using namespace std;

int main(){

    vector<int> A = {5, 2, 7, 3};

    SegmentTree tree(A);

    cout << "Suma de 0 a 3: " << tree.query(0, 3) << endl;
    cout << "Suma de 1 a 3: " << tree.query(1, 3) << endl;

    tree.update(3, 10);

    cout << "Despues del update:" << endl;
    cout << "Suma de 0 a 3: " << tree.query(0, 3) << endl;
    cout << "Suma de 1 a 3: " << tree.query(1, 3) << endl;

    return 0;
}
