#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

#include <stdexcept>
#include <vector>

class FenwickTree {
private:
    std::vector<long long> tree;
    int n;

    static int lowbit(int index) {
        return index & -index;
    }

public:
    explicit FenwickTree(int size) : n(size) {
        if (size < 0) {
            throw std::invalid_argument("El tamano no puede ser negativo");
        }
        tree.assign(size + 1, 0);
    }

    // Los indices validos de actualizacion son 1 ... n.
    void update(int index, long long delta) {
        if (index < 1 || index > n) {
            throw std::out_of_range("Indice Fenwick fuera de rango");
        }

        while (index <= n) {
            tree[index] += delta;
            index += lowbit(index);
        }
    }

    // Suma del prefijo [1, index]. query(0) = 0.
    long long query(int index) const {
        if (index < 0 || index > n) {
            throw std::out_of_range("Indice Fenwick fuera de rango");
        }

        long long sum = 0;
        while (index > 0) {
            sum += tree[index];
            index -= lowbit(index);
        }
        return sum;
    }

    long long query(int left, int right) const {
        if (left < 1 || right > n || left > right) {
            throw std::out_of_range("Rango Fenwick invalido");
        }
        return query(right) - query(left - 1);
    }

    int size() const {
        return n;
    }
};

#endif
