#include <iostream>
#include "Stack.h"

using namespace std;

int main(){

    Stack s;

    s.push(10);
    s.push(20);
    s.push(30);

    cout << "Top: " << s.top() << endl;

    s.pop();

    cout << "Top despues de pop: " << s.top() << endl;

    s.pop();

    cout << "Top despues de otro pop: " << s.top() << endl;

    return 0;
}