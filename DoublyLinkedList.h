#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
};

template <typename T>
class DoublyLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Destructor for manual memory management
    ~DoublyLinkedList() {
        clear();
    }

    // Copy Constructor
    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), size(0) {
        Node<T>* current = other.head;
        while (current) {
            insertBack(current->data);
            current = current->next;
        }
    }

    // Assignment Operator
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            clear();
            Node<T>* current = other.head;
            while (current) {
                insertBack(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    void insertBack(const T& val) {
        Node<T>* newNode = new Node<T>(val);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    void insertFront(const T& val) {
        Node<T>* newNode = new Node<T>(val);
        if (!head) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    void insertAt(int index, const T& val) {
        if (index < 0 || index > size) return;
        if (index == 0) {
            insertFront(val);
            return;
        }
        if (index == size) {
            insertBack(val);
            return;
        }

        Node<T>* newNode = new Node<T>(val);
        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        newNode->prev = current->prev;
        newNode->next = current;
        current->prev->next = newNode;
        current->prev = newNode;
        size++;
    }

    void removeAt(int index) {
        if (index < 0 || index >= size) return;

        Node<T>* toDelete = head;
        if (index == 0) {
            head = head->next;
            if (head) head->prev = nullptr;
            else tail = nullptr;
        }
        else if (index == size - 1) {
            toDelete = tail;
            tail = tail->prev;
            if (tail) tail->next = nullptr;
            else head = nullptr;
        }
        else {
            for (int i = 0; i < index; i++) {
                toDelete = toDelete->next;
            }
            toDelete->prev->next = toDelete->next;
            toDelete->next->prev = toDelete->prev;
        }

        delete toDelete;
        size--;
    }

    // Module 2 requirement: reverse() function to reorient the train
    void reverse() {
        if (!head || head == tail) return;

        Node<T>* current = head;
        Node<T>* temp = nullptr;

        while (current != nullptr) {
            // Swap next and prev pointers
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;

            // Move to the next node in the original sequence (which is now current->prev)
            current = current->prev;
        }

        // Swap head and tail
        temp = head;
        head = tail;
        tail = temp;
    }

    void clear() {
        Node<T>* current = head;
        while (current) {
            Node<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = tail = nullptr;
        size = 0;
    }

    T* getAt(int index) {
        if (index < 0 || index >= size) return nullptr;
        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return &(current->data);
    }

    void display() const {
        Node<T>* current = head;
        while (current) {
             cout << current->data << (current->next ? " <-> " : "");
            current = current->next;
        }
         cout <<  endl;
    }

    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }
    Node<T>* getHead() const { return head; }
};

#endif
