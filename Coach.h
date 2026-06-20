#ifndef COACH_H
#define COACH_H

#include <iostream>
#include <string>
#include <fstream>
#include "HashTable.h"
#include "AVLTree.h"
using namespace std;

struct Seat {
    int seatNumber;
     string passengerName;
    bool isOccupied;

    Seat() : seatNumber(0), passengerName("Empty"), isOccupied(false) {}
    Seat(int num, const  string& name) : seatNumber(num), passengerName(name), isOccupied(true) {}

    // Comparison for BST
    bool operator<(const Seat& other) const { return seatNumber < other.seatNumber; }
    bool operator>(const Seat& other) const { return seatNumber > other.seatNumber; }
    bool operator==(const Seat& other) const { return seatNumber == other.seatNumber; }

    friend  ostream& operator<<( ostream& os, const Seat& s) {
        os << s.seatNumber << " " << s.passengerName;
        return os;
    }

    void saveToFile( ofstream& os) const {
        os << seatNumber << " " << passengerName << " ";
    }
};

class Coach {
private:
     string coachType;
    HashTable<Seat> seatLookup; // Layer 1: Hashing for O(1) lookup
    AVLTree<Seat> seatRegistry;  // Layer 2: BST for ordered traversal

    // Valid seat range: 0 .. 49
    static constexpr int MIN_SEAT = 0;
    static constexpr int MAX_SEAT = 49;
    bool isValidSeatNumber(int num) const {
        return num >= MIN_SEAT && num <= MAX_SEAT;
    }

public:
    Coach() : coachType("Generic") {}
    Coach(const  string& type) : coachType(type) {}

    bool bookSeat(int num, const  string& name) {
        if (!isValidSeatNumber(num)) {
             cout << "[ERROR] Seat number must be between " << MIN_SEAT << " and " << MAX_SEAT << "." <<  endl;
            return false;
        }

        Seat* s = seatLookup.search(num);
        if (s) {
             cout << "[ERROR] Seat " << num << " is already booked by: " << s->passengerName <<  endl;
            return false;
        }
        Seat newSeat(num, name);
        seatLookup.insert(num, newSeat);
        seatRegistry.insert(newSeat);
        return true;
    }

    void findPassenger(int num) {
        if (!isValidSeatNumber(num)) {
             cout << "[ERROR] Seat number must be between " << MIN_SEAT << " and " << MAX_SEAT << "." <<  endl;
            return;
        }

        Seat* s = seatLookup.search(num);
        if (s) {
             cout << "Seat " << num << " is occupied by: " << s->passengerName <<  endl;
        }
        else {
             cout << "Seat " << num << " is currently empty." <<  endl;
        }
    }

    bool cancelSeat(int num) {
        if (!isValidSeatNumber(num)) {
             cout << "[ERROR] Seat number must be between " << MIN_SEAT << " and " << MAX_SEAT << "." <<  endl;
            return false;
        }

        Seat temp;
        temp.seatNumber = num;
        seatLookup.remove(num);
        seatRegistry.remove(temp);
        return true;
    }

    void displaySortedSeats() const {
         cout << "Coach (" << coachType << ") Seating Chart (Sorted): ";
        seatRegistry.display();
    }

     string getType() const { return coachType; }

    // Overload for DoublyLinkedList display
    friend  ostream& operator<<( ostream& os, const Coach& c) {
        os << c.coachType;
        return os;
    }

    void saveToFile( ofstream& outFile) {
        outFile << coachType << " " << seatRegistry.getSize() << "\n";
        seatRegistry.saveLevelOrder(outFile);
    }
};

#endif
