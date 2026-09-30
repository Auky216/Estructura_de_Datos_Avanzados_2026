#include <iostream>
#include "Persistent_Stack.h"

using namespace std;

int main(){
    PersistentStack stack;

    int version1 = stack.push(0, 10);
    int version2 = stack.push(version1, 20);
    int version3 = stack.push(version2, 30);
    int version4 = stack.pop(version3);

    // Tambien puede ramificarse desde una version antigua.
    int version5 = stack.push(version1, 99);

    cout << stack.top(version1) << '\n'; // 10
    cout << stack.top(version3) << '\n'; // 30
    cout << stack.top(version4) << '\n'; // 20
    cout << stack.top(version5) << '\n'; // 99

    return 0;
}

/*
Complejidad:
- push: O(1) tiempo y O(1) espacio nuevo.
- pop:  O(1) tiempo y O(1) espacio en el vector de raices; no crea nodos.
- top:  O(1) tiempo.
- roots[i] representa toda la pila en la version i.
*/
