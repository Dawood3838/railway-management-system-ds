
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
struct HeapNode {
    int vertex;
    T priority;
};

template <typename T>
class MinHeap {
private:
    HeapNode<T>* heap;
    int capacity;
    int size;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].priority < heap[parent].priority) {
                HeapNode<T> temp = heap[index];
                heap[index] = heap[parent];
                heap[parent] = temp;
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left].priority < heap[smallest].priority) {
                smallest = left;
            }
            if (right < size && heap[right].priority < heap[smallest].priority) {
                smallest = right;
            }

            if (smallest != index) {
                HeapNode<T> temp = heap[index];
                heap[index] = heap[smallest];
                heap[smallest] = temp;
                index = smallest;
            }
            else {
                break;
            }
        }
    }

public:
    MinHeap(int cap) : capacity(cap), size(0) {
        heap = new HeapNode<T>[capacity];
    }

    ~MinHeap() {
        delete[] heap;
    }

    void push(int v, T p) {
        if (size >= capacity) return;
        heap[size].vertex = v;
        heap[size].priority = p;
        heapifyUp(size);
        size++;
    }

    HeapNode<T> pop() {
        if (size == 0) throw  runtime_error("Heap empty");
        HeapNode<T> root = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
        return root;
    }

    bool isEmpty() const {
        return size == 0;
    }
};
