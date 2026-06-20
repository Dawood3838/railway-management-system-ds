#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <fstream>
#include "Queue.h"
using namespace std;

template <typename T>
struct AVLNode {
    T data;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const T& val) : data(val), height(1), left(nullptr), right(nullptr) {}
};

template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    int getHeight(AVLNode<T>* node) {
        return node ? node->height : 0;
    }

    int getBalance(AVLNode<T>* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    int myMax(int a, int b) {
        return (a > b) ? a : b;
    }

    AVLNode<T>* rotateRight(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = myMax(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = myMax(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    AVLNode<T>* rotateLeft(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = myMax(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = myMax(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    AVLNode<T>* insert(AVLNode<T>* node, const T& val) {
        if (!node) return new AVLNode<T>(val);

        if (val < node->data)
            node->left = insert(node->left, val);
        else if (val > node->data)
            node->right = insert(node->right, val);
        else
            return node; // Duplicate keys not allowed

        node->height = 1 + myMax(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && val < node->left->data)
            return rotateRight(node);

        // Right Right Case
        if (balance < -1 && val > node->right->data)
            return rotateLeft(node);

        // Left Right Case
        if (balance > 1 && val > node->left->data) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Left Case
        if (balance < -1 && val < node->right->data) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void clear(AVLNode<T>* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void displayInOrder(AVLNode<T>* node) const {
        if (node) {
            displayInOrder(node->left);
             cout << node->data << " ";
            displayInOrder(node->right);
        }
    }

    int getSize(AVLNode<T>* node) const {
        if (!node) return 0;
        return 1 + getSize(node->left) + getSize(node->right);
    }

    AVLNode<T>* search(AVLNode<T>* node, const T& val) const {
        if (!node || node->data == val)
            return node;

        if (val < node->data)
            return search(node->left, val);

        return search(node->right, val);
    }

    AVLNode<T>* copyNode(AVLNode<T>* otherNode) {
        if (!otherNode) return nullptr;
        AVLNode<T>* newNode = new AVLNode<T>(otherNode->data);
        newNode->height = otherNode->height;
        newNode->left = copyNode(otherNode->left);
        newNode->right = copyNode(otherNode->right);
        return newNode;
    }

public:
    AVLTree() : root(nullptr) {}

    // Copy Constructor
    AVLTree(const AVLTree& other) {
        root = copyNode(other.root);
    }

    // Assignment Operator
    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            clear(root);
            root = copyNode(other.root);
        }
        return *this;
    }

    AVLNode<T>* getRoot() const { return root; }

    AVLNode<T>* minValueNode(AVLNode<T>* node) {
        AVLNode<T>* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    AVLNode<T>* remove(AVLNode<T>* root, const T& val) {
        if (root == nullptr) return root;

        if (val < root->data)
            root->left = remove(root->left, val);
        else if (val > root->data)
            root->right = remove(root->right, val);
        else {
            // Node with only one child or no child
            if ((root->left == nullptr) || (root->right == nullptr)) {
                AVLNode<T>* temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp; // Copy contents of non-empty child

                delete temp;
            }
            else {
                // Node with two children: Get inorder successor
                AVLNode<T>* temp = minValueNode(root->right);
                root->data = temp->data;
                root->right = remove(root->right, temp->data);
            }
        }

        if (root == nullptr) return root;

        root->height = 1 + myMax(getHeight(root->left), getHeight(root->right));

        int balance = getBalance(root);

        // Left Left Case
        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);

        // Left Right Case
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);

        // Right Left Case
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    ~AVLTree() {
        clear(root);
    }

    void insert(const T& val) {
        root = insert(root, val);
    }

    void remove(const T& val) {
        root = remove(root, val);
    }

    T* search(const T& val) {
        AVLNode<T>* result = search(root, val);
        return result ? &(result->data) : nullptr;
    }

    void display() const {
        displayInOrder(root);
         cout <<  endl;
    }

    int getSize() const {
        return getSize(root);
    }

    // Phase 5 Bonus: Save in Level-Order to preserve hierarchy
    void saveLevelOrder( ofstream& outFile) {
        if (!root) return;

        Queue<AVLNode<T>*> q;
        q.enqueue(root);

        while (!q.isEmpty()) {
            AVLNode<T>* current = q.getFront();
            q.dequeue();

            // Save node data to file using custom save method
            current->data.saveToFile(outFile);

            if (current->left) q.enqueue(current->left);
            if (current->right) q.enqueue(current->right);
        }
        outFile << "\n";
    }
};

#endif
