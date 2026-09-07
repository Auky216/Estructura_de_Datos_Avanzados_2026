#include <iostream>
#include "FenwickTree.h"

using namespace std;

int main(){

    FenwickTree fenwick(8);

    fenwick.update(2, 1);
    fenwick.update(5, 1);
    fenwick.update(7, 1);

    cout << fenwick.query(5) << endl;

    cout << fenwick.query(3, 8) << endl;

    return 0;
}