#include <iostream>
#include <vector>

using namespace std;


// =====================================================
// NODO
// =====================================================

struct Node {

    int key;

    // Cantidad de hijos
    int degree;

    // true = ya perdió un hijo anteriormente
    bool mark;

    Node* parent;

    // Apunta a uno de sus hijos
    Node* child;

    // Lista doblemente enlazada circular
    Node* left;
    Node* right;


    Node(int _key) {

        key = _key;

        degree = 0;

        mark = false;

        parent = nullptr;

        child = nullptr;


        // Un nodo nuevo comienza siendo
        // una lista circular consigo mismo.

        left = this;
        right = this;
    }
};



// =====================================================
// FIBONACCI HEAP
// =====================================================

class FibonacciHeap {

private:

    // Raíz con menor key
    Node* minNode;

    // Cantidad total de nodos
    int n;



    // =================================================
    // AGREGAR UN NODO A LA LISTA DE RAÍCES
    // =================================================

    void addToRootList(Node* x) {

        // ---------------------------------------------
        // CASO 1:
        // El heap está vacío
        // ---------------------------------------------

        if (minNode == nullptr) {

            x->left = x;
            x->right = x;

            minNode = x;

            return;
        }


        // ---------------------------------------------
        // CASO 2:
        // Ya existen raíces
        // ---------------------------------------------

        /*
            Antes:

            minNode
               ↓

            5 -----> 8


            Queremos meter x entre ambos:

            5 -----> x -----> 8
        */


        // x.right apunta al que estaba
        // después de minNode

        x->right = minNode->right;


        // x.left apunta a minNode

        x->left = minNode;


        // El antiguo siguiente ahora
        // tiene a x a su izquierda

        minNode->right->left = x;


        // minNode ahora tiene x a su derecha

        minNode->right = x;


        // Por ahora x es una raíz

        x->parent = nullptr;


        // Si x es menor actualizamos el mínimo

        if (x->key < minNode->key) {

            minNode = x;
        }
    }



    // =================================================
    // LINK
    // =================================================

    /*
        Se usa durante Consolidate.

        Recibimos dos árboles del mismo grado.

        y se convierte en hijo de x.

        Ejemplo:

           2        5
          /        /
         8        9


        link(5,2)


             2
            / \
           8   5
               \
                9
    */

    void link(Node* y, Node* x) {

        // y ahora tendrá a x como padre

        y->parent = x;


        // Cuando un nodo pasa a ser hijo
        // comienza sin marca.

        y->mark = false;


        // ---------------------------------------------
        // CASO 1:
        // x todavía no tiene hijos
        // ---------------------------------------------

        if (x->child == nullptr) {

            x->child = y;


            // y forma una lista circular
            // de hijos consigo mismo

            y->left = y;
            y->right = y;
        }


        // ---------------------------------------------
        // CASO 2:
        // x ya tiene hijos
        // ---------------------------------------------

        else {

            Node* child = x->child;


            /*
                Insertamos y en la lista
                circular de hijos de x.
            */

            y->right = child->right;

            y->left = child;

            child->right->left = y;

            child->right = y;
        }


        // x consiguió un nuevo hijo

        x->degree++;
    }



    // =================================================
    // CONSOLIDATE
    // =================================================

    /*
        Objetivo:

        NO dejar dos raíces con el mismo grado.

        A[d] guarda una raíz de grado d.

        Si aparece otra raíz con el mismo grado:

        hacemos LINK.
    */

    void consolidate() {

        if (minNode == nullptr) {
            return;
        }


        // =================================================
        // PASO 1:
        // Guardamos todas las raíces actuales
        // =================================================

        vector<Node*> roots;


        Node* current = minNode;


        do {

            roots.push_back(current);

            current = current->right;

        } while (current != minNode);



        /*
            Como ya guardamos todas las raíces
            en el vector, podemos separarlas
            temporalmente.
        */

        for (Node* root : roots) {

            root->left = root;
            root->right = root;
        }



        // A[d] = raíz con grado d

        vector<Node*> A(1, nullptr);



        // =================================================
        // PASO 2:
        // Procesamos cada raíz
        // =================================================

        for (Node* root : roots) {

            Node* x = root;

            int d = x->degree;



            // Si necesitamos más posiciones,
            // hacemos crecer el vector.

            while (d >= static_cast<int>(A.size())) {

                A.resize(d + 1, nullptr);
            }



            // Mientras ya exista otra raíz
            // con el mismo grado...

            while (A[d] != nullptr) {

                Node* y = A[d];


                /*
                    Queremos que la raíz menor
                    quede arriba.

                    Si:

                    x.key > y.key

                    intercambiamos.
                */

                if (x->key > y->key) {

                    Node* temp = x;
                    x = y;
                    y = temp;
                }



                /*
                    y se convierte en hijo de x
                */

                link(y, x);



                // Ya usamos esta posición

                A[d] = nullptr;


                // x ganó un hijo
                // entonces su grado aumentó

                d++;


                // Aseguramos espacio

                while (d >= static_cast<int>(A.size())) {

                    A.resize(d + 1, nullptr);
                }
            }



            // Guardamos x según su nuevo grado

            A[d] = x;
        }



        // =================================================
        // PASO 3:
        // Reconstruimos la lista de raíces
        // =================================================

        minNode = nullptr;


        for (Node* x : A) {

            if (x != nullptr) {

                x->left = x;
                x->right = x;

                x->parent = nullptr;


                addToRootList(x);
            }
        }
    }



    // =================================================
    // CUT
    // =================================================

    /*
        Antes:

            y
           /
          x
         / \
        ... ...


        Después:

        y          x
                  / \
                 ... ...

        IMPORTANTE:

        x se mueve junto con TODO su subárbol.
    */

    void cut(Node* x, Node* y) {


        // ---------------------------------------------
        // CASO 1:
        // x era el único hijo de y
        // ---------------------------------------------

        if (x->right == x) {

            y->child = nullptr;
        }


        // ---------------------------------------------
        // CASO 2:
        // y tiene varios hijos
        // ---------------------------------------------

        else {

            // Si y.child apuntaba precisamente a x,
            // debemos cambiarlo a otro hijo.

            if (y->child == x) {

                y->child = x->right;
            }


            // Sacamos x de la lista circular
            // de hijos.

            x->left->right = x->right;

            x->right->left = x->left;
        }



        // y perdió un hijo

        y->degree--;



        // Ahora x deja de tener padre

        x->parent = nullptr;


        // Al convertirse en raíz
        // deja de estar marcado

        x->mark = false;



        // Primero dejamos x aislado

        x->left = x;
        x->right = x;



        // Después lo agregamos
        // a las raíces

        addToRootList(x);
    }



    // =================================================
    // CASCADING CUT
    // =================================================

    void cascadingCut(Node* y) {


        // Padre de y

        Node* z = y->parent;



        // Si y es raíz,
        // terminamos.

        if (z == nullptr) {

            return;
        }



        // ---------------------------------------------
        // CASO 1:
        // y nunca había perdido un hijo
        // ---------------------------------------------

        if (y->mark == false) {

            /*
                Perdió su primer hijo.

                NO lo cortamos.

                Solo lo marcamos.
            */

            y->mark = true;
        }


        // ---------------------------------------------
        // CASO 2:
        // y ya estaba marcado
        // ---------------------------------------------

        else {

            /*
                Ya había perdido un hijo.

                Ahora perdió otro.

                Entonces:
            */

            cut(y, z);


            /*
                Ahora revisamos al padre z.

                Esto produce el posible
                corte en cascada.
            */

            cascadingCut(z);
        }
    }



public:


    // =================================================
    // CONSTRUCTOR
    // =================================================

    FibonacciHeap() {

        minNode = nullptr;

        n = 0;
    }

    ~FibonacciHeap() {
        while (minNode != nullptr) {
            extractMin();
        }
    }

    FibonacciHeap(const FibonacciHeap&) = delete;
    FibonacciHeap& operator=(const FibonacciHeap&) = delete;



    // =================================================
    // INSERT
    // =================================================

    Node* insert(int key) {


        // Creamos nodo

        Node* x = new Node(key);



        // Lo agregamos directamente
        // como nueva raíz

        addToRootList(x);



        // Aumentamos cantidad de nodos

        n++;



        /*
            NO hacemos Consolidate.

            Fibonacci Heap es "perezoso".
        */


        return x;
    }



    // =================================================
    // GET MIN
    // =================================================

    int getMin() {


        if (minNode == nullptr) {

            cout << "Heap vacio" << endl;

            return -1;
        }


        return minNode->key;
    }



    // =================================================
    // UNION
    // =================================================

    void unionHeap(FibonacciHeap& other) {

        if (this == &other) {
            return;
        }


        // ---------------------------------------------
        // CASO 1:
        // El otro heap está vacío
        // ---------------------------------------------

        if (other.minNode == nullptr) {

            return;
        }



        // ---------------------------------------------
        // CASO 2:
        // Nuestro heap está vacío
        // ---------------------------------------------

        if (minNode == nullptr) {

            minNode = other.minNode;

            n = other.n;


            // Dejamos el otro heap vacío

            other.minNode = nullptr;
            other.n = 0;

            return;
        }



        // ---------------------------------------------
        // CASO 3:
        // Ambos tienen raíces
        // ---------------------------------------------


        /*
            H1:

            2 <-> 8


            H2:

            1 <-> 7 <-> 9


            Queremos:

            2 <-> 8 <-> 1 <-> 7 <-> 9
        */


        Node* firstAfterMin = minNode->right;

        Node* lastOther = other.minNode->left;



        /*
            Conectamos:

            minNode
                ↓

            A ----> B
        */

        minNode->right = other.minNode;

        other.minNode->left = minNode;



        /*
            Cerramos la conexión del otro extremo.
        */

        lastOther->right = firstAfterMin;

        firstAfterMin->left = lastOther;



        // Elegimos el menor mínimo

        if (other.minNode->key < minNode->key) {

            minNode = other.minNode;
        }



        // Sumamos tamaños

        n += other.n;



        /*
            Transferimos todos los nodos.

            other queda vacío.
        */

        other.minNode = nullptr;

        other.n = 0;
    }



    // =================================================
    // DECREASE KEY
    // =================================================

    void decreaseKey(Node* x, int newKey) {

        if (x == nullptr) {
            return;
        }


        // ---------------------------------------------
        // CASO INVÁLIDO
        // ---------------------------------------------

        if (newKey > x->key) {

            cout << "La nueva llave debe ser menor" << endl;

            return;
        }



        // Cambiamos la llave

        x->key = newKey;



        // Padre de x

        Node* y = x->parent;



        // ---------------------------------------------
        // ¿ROMPIÓ LA PROPIEDAD DEL MIN-HEAP?
        // ---------------------------------------------

        if (
            y != nullptr &&
            x->key < y->key
        ) {


            // Sacamos x de y

            cut(x, y);



            // Revisamos si y también
            // debe ser cortado

            cascadingCut(y);
        }



        // Actualizamos mínimo si hace falta

        if (
            minNode == nullptr ||
            x->key < minNode->key
        ) {

            minNode = x;
        }
    }



    // =================================================
    // EXTRACT MIN
    // =================================================

    int extractMin() {


        Node* z = minNode;



        // ---------------------------------------------
        // CASO:
        // Heap vacío
        // ---------------------------------------------

        if (z == nullptr) {

            cout << "Heap vacio" << endl;

            return -1;
        }



        // Guardamos el valor que vamos a retornar

        int result = z->key;



        // =================================================
        // PASO 1:
        // Sus hijos pasan a ser raíces
        // =================================================

        if (z->child != nullptr) {


            vector<Node*> children;


            Node* current = z->child;



            // Primero guardamos los hijos

            do {

                children.push_back(current);

                current = current->right;

            } while (current != z->child);



            // Ahora los movemos a raíces

            for (Node* child : children) {


                // Ya no tienen padre

                child->parent = nullptr;


                // Las raíces no quedan marcadas

                child->mark = false;


                // Lo aislamos de la lista de hijos

                child->left = child;
                child->right = child;


                // Lo agregamos a las raíces

                addToRootList(child);
            }



            z->child = nullptr;

            z->degree = 0;
        }



        // =================================================
        // PASO 2:
        // Eliminamos z de la lista de raíces
        // =================================================


        // Si z sigue siendo el único nodo...

        if (z->right == z) {

            minNode = nullptr;
        }


        else {

            // Guardamos otro nodo
            // antes de desconectar z

            Node* next = z->right;



            // Sacamos z de la lista

            z->left->right = z->right;

            z->right->left = z->left;



            // Temporalmente min apunta
            // a cualquier raíz restante

            minNode = next;



            // =================================================
            // PASO 3:
            // CONSOLIDATE
            // =================================================

            consolidate();
        }



        // Un nodo menos

        n--;



        // Liberamos el nodo mínimo

        delete z;



        return result;
    }



    // =================================================
    // IMPRIMIR RAÍCES
    // =================================================

    void printRoots() {


        if (minNode == nullptr) {

            cout << "Heap vacio" << endl;

            return;
        }



        Node* current = minNode;


        cout << "Raices: ";


        do {

            cout << current->key
                 << "(grado "
                 << current->degree
                 << ") ";

            current = current->right;

        } while (current != minNode);



        cout << endl;


        cout << "Minimo: "
             << minNode->key
             << endl;
    }

    bool empty() const {
        return minNode == nullptr;
    }

    int size() const {
        return n;
    }
};



// =====================================================
// MAIN
// =====================================================

int main() {


    FibonacciHeap heap;



    // =================================================
    // INSERT
    // =================================================

    heap.insert(5);

    heap.insert(3);

    heap.insert(9);

    Node* n7 = heap.insert(7);


    cout << "Despues de insertar:" << endl;

    heap.printRoots();



    /*
        Tenemos aproximadamente:

        3 <-> 7 <-> 9 <-> 5

        min = 3

        Todos son raíces.
    */


    cout << endl;



    // =================================================
    // EXTRACT MIN
    // =================================================

    cout << "Extract-Min: "
         << heap.extractMin()
         << endl;


    /*
        Se elimina 3.

        Después entra Consolidate.

        Las raíces con el mismo grado
        comienzan a unirse.
    */


    heap.printRoots();



    cout << endl;



    // =================================================
    // DECREASE KEY
    // =================================================

    /*
        n7 sigue siendo un puntero al nodo 7.

        Si después de Consolidate tiene padre
        y hacemos que su llave sea muy pequeña:

        puede ocurrir CUT.
    */


    heap.decreaseKey(n7, 1);


    cout << "Despues de Decrease-Key:" << endl;

    heap.printRoots();



    cout << endl;



    // =================================================
    // UNION
    // =================================================

    FibonacciHeap heap2;


    heap2.insert(20);

    heap2.insert(2);

    heap2.insert(15);


    cout << "Segundo heap:" << endl;

    heap2.printRoots();


    cout << endl;


    heap.unionHeap(heap2);


    cout << "Despues de Union:" << endl;

    heap.printRoots();



    return 0;
}
