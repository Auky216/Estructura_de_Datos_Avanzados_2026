#ifndef STACK_H
#define STACK_H

struct Node {
    int value;
    Node* next;

    Node(int value, Node* next = nullptr) : value(value), next(next) {}
};

class Stack {
private:
    Node* head;
    int numberOfElements;

    void clear() {
        while (head != nullptr) {
            Node* temporary = head;
            head = head->next;
            delete temporary;
        }
        numberOfElements = 0;
    }

public:
    Stack() : head(nullptr), numberOfElements(0) {}

    ~Stack() {
        clear();
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(int value) {
        head = new Node(value, head);
        ++numberOfElements;
    }

    void pop() {
        if (head == nullptr) {
            return;
        }

        Node* temporary = head;
        head = head->next;
        delete temporary;
        --numberOfElements;
    }

    int top() const {
        return head == nullptr ? -1 : head->value;
    }

    bool empty() const {
        return head == nullptr;
    }

    int size() const {
        return numberOfElements;
    }
};

#endif
