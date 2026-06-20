#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <string>
using namespace std;

template <typename T>
struct HashNode {
    int key;
    T value;
    HashNode* next;

    HashNode(int k, const T& v) : key(k), value(v), next(nullptr) {}
};

template <typename T>
class HashTable {
private:
    HashNode<T>** table;
    int capacity;

    int hashFunction(int key) {
        return key % capacity;
    }

public:
    HashTable(int cap = 50) : capacity(cap) {
        table = new HashNode<T>*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }

    // Copy Constructor
    HashTable(const HashTable& other) : capacity(other.capacity) {
        table = new HashNode<T>*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
            HashNode<T>* current = other.table[i];
            while (current) {
                insert(current->key, current->value);
                current = current->next;
            }
        }
    }

    // Assignment Operator
    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            // Free existing
            for (int i = 0; i < capacity; i++) {
                HashNode<T>* current = table[i];
                while (current) {
                    HashNode<T>* next = current->next;
                    delete current;
                    current = next;
                }
            }
            delete[] table;

            // Copy new
            capacity = other.capacity;
            table = new HashNode<T>*[capacity];
            for (int i = 0; i < capacity; i++) {
                table[i] = nullptr;
                HashNode<T>* current = other.table[i];
                while (current) {
                    insert(current->key, current->value);
                    current = current->next;
                }
            }
        }
        return *this;
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            HashNode<T>* current = table[i];
            while (current) {
                HashNode<T>* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] table;
    }

    void insert(int key, const T& value) {
        int index = hashFunction(key);
        HashNode<T>* newNode = new HashNode<T>(key, value);

        if (!table[index]) {
            table[index] = newNode;
        }
        else {
            // Chaining for collision handling
            newNode->next = table[index];
            table[index] = newNode;
        }
    }

    T* search(int key) {
        int index = hashFunction(key);
        HashNode<T>* current = table[index];
        while (current) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    void remove(int key) {
        int index = hashFunction(key);
        HashNode<T>* current = table[index];
        HashNode<T>* prev = nullptr;

        while (current) {
            if (current->key == key) {
                if (prev) prev->next = current->next;
                else table[index] = current->next;
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }
};

#endif
