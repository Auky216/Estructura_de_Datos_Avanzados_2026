# Proyecto 1: Monticulo de minimos

## Diseno de la solucion

El programa implementa un monticulo de minimos para representar el planificador de tareas solicitado. Cada elemento se guarda en un `Node` con dos campos: `id`, que identifica de forma unica a la tarea, y `value`, que representa su clave o prioridad. Una prioridad menor representa una tarea mas urgente. Cuando dos tareas tienen la misma prioridad, la tarea con menor identificador tiene prioridad, tal como exige el enunciado.

La estructura `MinHeap` usa arreglos dinamicos propios y no emplea `priority_queue`, `make_heap` ni otro contenedor nativo que implemente un heap. El arreglo `array` almacena el monticulo desde la posicion 1 para que el padre de un nodo en la posicion `i` sea `i / 2`, y sus hijos sean `2 * i` y `2 * i + 1`. El campo `size` indica la cantidad actual de elementos y `capacity` limita el arreglo al maximo posible de elementos, `n + q`.

Ademas se mantiene el arreglo `position`. Para cada identificador presente, `position[id]` guarda el indice donde se encuentra su nodo dentro del heap. Si el elemento ya fue retirado, su posicion es `-1`. Este arreglo permite encontrar directamente un elemento al ejecutar una operacion de decremento de clave; por ello no se requiere recorrer todo el heap.

La comparacion de prioridades esta concentrada en la funcion `hasHigherPriority`. Primero compara `value`; si los valores son iguales compara `id`. De esta forma el desempate por menor identificador se aplica de manera uniforme durante las inserciones, las extracciones y las restauraciones del heap.

## Operaciones implementadas

La operacion `insert(id, value)` agrega el nuevo nodo al final del arreglo, actualiza su posicion y ejecuta `bubbleUp`. Esta ultima funcion compara el nodo con su padre y los intercambia mientras el hijo tenga mayor prioridad. En el programa principal los elementos iniciales reciben los ids de 1 a `n`; cada operacion de tipo 1 usa `nextId`, que comienza en `n + 1` y aumenta despues de cada insercion.

La operacion `extractMin()` obtiene el nodo de la raiz, que es el elemento de mayor prioridad. Guarda su identificador para imprimirlo, marca su posicion como `-1`, mueve el ultimo elemento a la raiz y disminuye el tamano. Finalmente, llama a `minHeapify(1)` para restaurar el orden hacia abajo. `minHeapify` escoge entre el nodo actual y sus hijos el que tenga mayor prioridad; si uno de los hijos debe subir, los intercambia y continua desde esa posicion.

La operacion `decreaseKey(id, newValue)` usa `position[id]` para llegar en tiempo constante al nodo pedido. Reemplaza su clave por el nuevo valor y ejecuta `bubbleUp`, porque al disminuir una clave el unico posible incumplimiento de la propiedad de heap esta entre el nodo y su padre. El enunciado garantiza que el id indicado permanece en la estructura y que la nueva clave es estrictamente menor, por lo que no se necesita manejar un incremento de clave.

La funcion `swapNodes` intercambia dos nodos del arreglo y actualiza inmediatamente las dos entradas correspondientes en `position`. Esta actualizacion es indispensable: despues de cada movimiento, las siguientes operaciones `decreaseKey` deben seguir localizando el elemento correcto.

## Correctitud

Se mantiene el siguiente invariante: para todo nodo del heap, su prioridad es menor o igual que la prioridad de cada hijo, usando el id como segundo criterio de comparacion. Por tanto, la raiz siempre es exactamente el elemento con menor clave y, en caso de empate, con menor id.

Al insertar, antes de `bubbleUp` el nuevo nodo solo puede violar el invariante con su padre. Cada intercambio lo mueve a una posicion cuyo subarbol ya respetaba el invariante; al terminar, el nodo no tiene mayor prioridad que su padre, por lo que todo el heap vuelve a ser valido. El mismo argumento aplica a `decreaseKey`, ya que la clave solo disminuye.

Al extraer la raiz, el ultimo nodo se mueve a la primera posicion. Sus subarboles continúan siendo heaps, pero puede violar el invariante con alguno de sus hijos. `minHeapify` intercambia el nodo con el hijo de mayor prioridad. Esto repara el invariante en la posicion anterior y desplaza el posible problema a un nivel inferior. Cuando no hay un hijo con mayor prioridad, el invariante queda restaurado. Como la raiz era menor o igual que todos los nodos por el invariante inicial, el id devuelto por `extractMin` es el requerido por la operacion de tipo 2.

## Complejidad

El arreglo de posiciones permite localizar un id en tiempo `O(1)`. La insercion y el decremento de clave recorren a lo sumo la altura del heap mediante `bubbleUp`, por lo que cuestan `O(log m)`, donde `m` es el numero de elementos presentes. La extraccion tambien cuesta `O(log m)` por `minHeapify`. La inicializacion de los `n` elementos mediante inserciones cuesta `O(n log n)`. El uso de memoria es `O(n + q)`: se reservan espacios para todos los elementos que podrian llegar a insertarse y para sus posiciones.

## Verificacion

Se compilo el programa con `c++ -std=c++17 -Wall -Wextra -pedantic`. Tambien se probaron los dos ejemplos del enunciado. El primer ejemplo imprime los ids `4` y `3`; el segundo imprime `1` y `2`, incluyendo el caso de empate de prioridades resuelto por el menor id.
