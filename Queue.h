#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
struct QueueNode {
    T data;
    QueueNode* next;

    QueueNode(const T& val) : data(val), next(nullptr) {}
};

template <typename T>
class Queue {
private:
    QueueNode<T>* front;
    QueueNode<T>* rear;
    int size;

public:
    Queue() : front(nullptr), rear(nullptr), size(0) {}

    ~Queue() {
        clear();
    }

    // Copy Constructor
    Queue(const Queue& other) : front(nullptr), rear(nullptr), size(0) {
        QueueNode<T>* current = other.front;
        while (current) {
            enqueue(current->data);
            current = current->next;
        }
    }

    // Assignment Operator
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            clear();
            QueueNode<T>* current = other.front;
            while (current) {
                enqueue(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    void enqueue(const T& val) {
        QueueNode<T>* newNode = new QueueNode<T>(val);
        if (!rear) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    void dequeue() {
        if (isEmpty()) throw  runtime_error("Queue empty");
        QueueNode<T>* temp = front;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        size--;
    }

    T getFront() const {
        if (isEmpty()) throw  runtime_error("Queue empty");
        return front->data;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

#endif
