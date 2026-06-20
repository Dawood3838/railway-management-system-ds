#ifndef TRAIN_H
#define TRAIN_H

#include <iostream>
#include "Coach.h"
#include "DoublyLinkedList.h"
using namespace std;

struct Train {
    int trainID;
     string name;
    DoublyLinkedList<Coach> coaches;

    Train() : trainID(0), name("Unknown") {}
    Train(int id,  string n) : trainID(id), name(n) {}

    // Comparison operators for AVL Tree search efficiency
    bool operator<(const Train& other) const { return trainID < other.trainID; }
    bool operator>(const Train& other) const { return trainID > other.trainID; }
    bool operator==(const Train& other) const { return trainID == other.trainID; }

    void addCoach(const Coach& coach) {
        coaches.insertBack(coach);
    }

    void addCoachAt(int index, const Coach& coach) {
        coaches.insertAt(index, coach);
    }

    void removeCoach(int index) {
        coaches.removeAt(index);
    }

    Coach* getCoach(int index) {
        return coaches.getAt(index);
    }

    void reverseCoaches() {
        coaches.reverse();
    }

    void displayTrain() const {
         cout << "Train [ID: " << trainID << " | Name: " << name << "]\nCoaches: ";
        coaches.display();
    }

    // Friend function to allow AVLTree to save Train data to file
    friend  ostream& operator<<( ostream& os, const Train& t) {
        os << t.trainID << " " << t.name;
        return os;
    }

    void saveToFile( ofstream& outFile) {
        outFile << trainID << " " << name << " " << coaches.getSize() << "\n";
        Node<Coach>* curr = coaches.getHead();
        while (curr) {
            curr->data.saveToFile(outFile);
            curr = curr->next;
        }
    }
};

#endif
