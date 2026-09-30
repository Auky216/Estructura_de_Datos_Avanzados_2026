# Indice de estructuras

## Estructuras mutables

| Carpeta | Operaciones principales | Complejidad |
|---|---|---|
| `Stack` | `push`, `pop`, `top`, `empty`, `size` | O(1) por operacion |
| `BinaryTree` | `insert`, `remove`, `contains`, `inorder` | O(h), recorrido O(n) |
| `Tries` | `insert`, `search`, `startsWith`, `erase` | O(longitud de la cadena) |
| `FenwickTree` | suma de prefijo, suma de rango, actualizacion | O(log n) |
| `SegmntTree` | suma de rango y actualizacion puntual | O(log n) |
| `MinHeap` | insertar, consultar y extraer minimo | O(log n), minimo O(1) |
| `Fibonacci_Heap` | insertar, unir, disminuir llave, extraer minimo | costos amortizados del Fibonacci Heap |

La carpeta conserva el nombre original `SegmntTree` para no romper rutas existentes.

## Estructuras persistentes

El archivo `PERSISTENCIA_README.md` contiene el orden recomendado de estudio y
el indice completo. Todas usan Path Copying y mantienen intactas las versiones
anteriores.

## Proyecto_1

`Proyecto_1` contiene dos implementaciones completas para el planificador:

- `PairingHeap.h`: implementacion usada actualmente por `main.cpp`.
- `MinHeap.h`: alternativa con arreglo de posiciones para `decreaseKey` en O(log n).

`tests.cpp` verifica insercion, extraccion, disminucion de llave y desempate por id.
