#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
struct StackNode {
    T data;
    StackNode* next;

    StackNode(const T& val) : data(val), next(nullptr) {}
};

template <typename T>
class Stack {
private:
    StackNode<T>* topNode;
    int size;

public:
    Stack() : topNode(nullptr), size(0) {}

    ~Stack() {
        clear();
    }

    // Copy Constructor
    Stack(const Stack& other) : topNode(nullptr), size(0) {
        if (other.topNode) {
            // Need to copy in order, so use a temp array or recursive approach
            // Simplest: use another stack to reverse
            Stack<T> temp;
            StackNode<T>* current = other.topNode;
            while (current) {
                temp.push(current->data);
                current = current->next;
            }
            while (!temp.isEmpty()) {
                push(temp.peek());
                temp.pop();
            }
        }
    }

    // Assignment Operator
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            Stack<T> temp;
            StackNode<T>* current = other.topNode;
            while (current) {
                temp.push(current->data);
                current = current->next;
            }
            while (!temp.isEmpty()) {
                push(temp.peek());
                temp.pop();
            }
        }
        return *this;
    }

    void push(const T& val) {
        StackNode<T>* newNode = new StackNode<T>(val);
        newNode->next = topNode;
        topNode = newNode;
        size++;
    }

    void pop() {
        if (isEmpty()) {
            throw  runtime_error("Stack Underflow");
        }
        StackNode<T>* temp = topNode;
        topNode = topNode->next;
        delete temp;
        size--;
    }

    T peek() const {
        if (isEmpty()) {
            throw  runtime_error("Stack is empty");
        }
        return topNode->data;
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }

    int getSize() const {
        return size;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};

#endif
