#include <iostream>
#include "Persistent_Stack.h"

using namespace std;

int main(){

    Persistent_Stack s;

    cout << "Version actual: " << s.top_version() << endl;

    s.push(10);
    cout << "Top: " << s.top() << endl;
    cout << "Version actual: " << s.top_version() << endl;

    s.push(20);
    cout << "Top: " << s.top() << endl;
    cout << "Version actual: " << s.top_version() << endl;

    s.push(30);
    cout << "Top: " << s.top() << endl;
    cout << "Version actual: " << s.top_version() << endl;

    s.pop();
    cout << "Top despues de pop: " << s.top() << endl;
    cout << "Version actual: " << s.top_version() << endl;

    return 0;
}