#include <iostream>
#include "BinaryTree.h"

using namespace std;

int main() {
    BinaryTree tree;

    // Insertar nodos
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    cout << "Nodos insertados correctamente." << endl;

    // Caso 1: eliminar hoja
    tree.remove(20);
    cout << "Eliminado 20." << endl;

    // Caso 2: eliminar nodo con un hijo
    tree.remove(30);
    cout << "Eliminado 30." << endl;

    // Caso 3: eliminar nodo con dos hijos
    tree.remove(70);
    cout << "Eliminado 70." << endl;

    // Eliminar la raiz
    tree.remove(50);
    cout << "Eliminado 50." << endl;

    // Valor inexistente
    tree.remove(100);
    cout << "Intento de eliminar 100." << endl;

    return 0;
}