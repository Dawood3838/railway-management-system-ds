#include <iostream>
#include <string>
#include <fstream>
#include "pch.h"
#include "Train.h"
#include "AVLTree.h"
#include "Stack.h"
#include "Graph.h"
using namespace std;

// Windows: enable ANSI color codes in cmd.exe / Windows Terminal
#ifdef _WIN32
  #include <windows.h>
  void enableWindowsANSI() {
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      DWORD dwMode = 0;
      GetConsoleMode(hOut, &dwMode);
      SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
      SetConsoleOutputCP(CP_UTF8);
  }
#else
  void enableWindowsANSI() {}
#endif

// ============================================================
//  ANSI COLOR PALETTE  (NetSentinel-style)
// ============================================================
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

// Foreground colours
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Bright variants
#define BMAGENTA    "\033[95m"
#define BCYAN       "\033[96m"
#define BGREEN      "\033[92m"
#define BRED        "\033[91m"
#define BYELLOW     "\033[93m"
#define BWHITE      "\033[97m"

// ============================================================
//  GLOBAL SYSTEM COMPONENTS
// ============================================================
AVLTree<Train> registry;
Graph network(0);
Stack<string> sessionLogs;
Stack<string> undoStack;

// ============================================================
//  VISUAL HELPERS
// ============================================================

// Full-width divider line (cyan, like NetSentinel separators)
void divider() {
    cout << CYAN << DIM
         << "------------------------------------------------------------"
         << RESET << endl;
}

// Phase-style centered section header
void phaseHeader(const string& label) {
    cout << endl;
    cout << CYAN << DIM
         << "------------------------------"
         << RESET
         << BOLD << MAGENTA << "  " << label << "  " << RESET
         << CYAN << DIM
         << "------------------------------"
         << RESET << endl;
}

// Prints the giant ASCII-art banner (shown once at startup)
// Pure ASCII only -- no Unicode block chars (Windows terminal safe)
void printAsciiLogo() {
    cout << BOLD << BMAGENTA;
    cout << "\n";
    cout << "  ____  ____  ___  _  _     _  _  ____  _  _  __  ___\n";
    cout << " (_  _)(  _ \\/ _ \\( \\( )   ( \\( )( ___)( \\/ )(  )/ __)\n";
    cout << "   )(   )   / (_) ))  (     )  (  )__)  )  (  )(__\\__ \\\n";
    cout << "  (__) (_)\\_)\\___/(_)\\_)   (_)\\_)(____)(_/\\_)(____)(___/\n";
    cout << RESET;
    cout << "\n";
    cout << BOLD << BMAGENTA;
    cout << BOLD << BMAGENTA;
    cout << "\n";
    cout << "  ╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "  ║                                                                                                                                                         ║\n";
    cout << "  ║  ██████╗  █████╗ ██╗██╗     ██╗    ██╗ █████╗ ██╗   ██╗   ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗███╗   ███╗███████╗███╗   ██╗████████╗  ║\n";
    cout << "  ║  ██╔══██╗██╔══██╗██║██║     ██║    ██║██╔══██╗╚██╗ ██╔╝   ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝  ║\n";
    cout << "  ║  ██████╔╝███████║██║██║     ██║ █╗ ██║███████║ ╚████╔╝    ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║     ║\n";
    cout << "  ║  ██╔══██╗██╔══██║██║██║     ██║███╗██║██╔══██║  ╚██╔╝     ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║     ║\n";
    cout << "  ║  ██║  ██║██║  ██║██║███████╗╚███╔███╔╝██║  ██║   ██║      ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║     ║\n";                                 
    cout << "  ║  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚══════╝ ╚══╝╚══╝ ╚═╝  ╚═╝   ╚═╝      ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝     ║\n";
    cout << "  ║                                                                                                                                                         ║\n";
    cout << "  ║                                                ███████╗██╗   ██╗███████╗████████╗ ███████╗ ███╗   ███╗                                                  ║\n";
    cout << "  ║                                                ██╔════╝╚██╗ ██╔╝██╔════╝╚══██╔══╝ ██╔════╝ ████╗ ████║                                                  ║\n";
    cout << "  ║                                                ███████╗ ╚████╔╝ ███████╗   ██║    █████╗   ██╔████╔██║                                                  ║\n";
    cout << "  ║                                                ╚════██║  ╚██╔╝  ╚════██║   ██║    ██╔══╝   ██║╚██╔╝██║                                                  ║\n";
    cout << "  ║                                                ███████║   ██║   ███████║   ██║    ███████╗ ██║ ╚═╝ ██║                                                  ║\n";
    cout << "  ║                                                ╚══════╝   ╚═╝   ╚══════╝   ╚═╝    ╚══════╝ ╚═╝     ╚═╝                                                  ║\n";
    cout << "  ║                                                                                                                                                         ║\n";
    cout << "  ╠═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n";
    cout << "  ║                                                          RAILWAY MANAGEMENT SYSTEM - COMMAND CENTER                                                     ║\n";
    cout << "  ╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n";
    cout << RESET;
    cout << RESET;
    cout << "\n";
    cout << BOLD << BCYAN;
    cout << "           Railway Management System  v1.0\n";
    cout << RESET << DIM << CYAN;
    cout << "           THE IRON NEXUS  --  Command Center\n";
    cout << RESET << "\n";
}

// Section header for sub-menus (like "Phase 2 - Analysis")
void sectionHeader(int phase, const string& label) {
    cout << endl;
    cout << CYAN << DIM;
    cout << "================================================================" << RESET << endl;
    cout << BOLD << CYAN;
    cout << "  Phase " << phase << "  --  " << label << RESET << endl;
    cout << CYAN << DIM;
    cout << "================================================================" << RESET << endl;
}

// Compact inline header for sub-menu screens
void printBanner(const string& text) {
    phaseHeader(text);
}

// Labelled prompt line
void prompt(const string& label) {
    cout << BCYAN << "  >> " << BWHITE << label << RESET;
}

// Status tags identical to NetSentinel style
void tagSuccess(const string& msg) {
    cout << BGREEN  << "  [SUCCESS] " << RESET << GREEN  << msg << RESET << endl;
}
void tagError(const string& msg) {
    cout << BRED    << "  [ERROR]   " << RESET << RED    << msg << RESET << endl;
}
void tagInfo(const string& msg) {
    cout << BCYAN   << "  [INFO]    " << RESET << CYAN   << msg << RESET << endl;
}
void tagWarn(const string& msg) {
    cout << BYELLOW << "  [WARN]    " << RESET << YELLOW << msg << RESET << endl;
}
void tagLocated(const string& msg) {
    cout << BGREEN  << "  [LOCATED] " << RESET << GREEN  << msg << RESET << endl;
}
void tagUndo(const string& msg) {
    cout << BMAGENTA<< "  [UNDO]    " << RESET << MAGENTA<< msg << RESET << endl;
}

// Menu item line: "  [ 1 ]  Register New Train"
void menuItem(int num, const string& label) {
    cout << "  " << CYAN << "[" << BWHITE << " " << num << " " << CYAN << "]"
         << RESET << "  " << WHITE << label << RESET << endl;
}

// Table row helpers (like NetSentinel's Phase 3 results table)
void tableHeader(const string& col1, const string& col2, const string& col3 = "", const string& col4 = "") {
    cout << BOLD << MAGENTA;
    cout << "  +-----------------------+---------------------------+";
    if (!col3.empty()) cout << "-------------------+";
    if (!col4.empty()) cout << "-----------+";
    cout << RESET << endl;

    cout << "  " << BOLD << MAGENTA << "| " << RESET
         << BWHITE << left;
    cout.width(22); cout << col1;
    cout << MAGENTA << "| " << RESET << BWHITE;
    cout.width(26); cout << col2;
    if (!col3.empty()) { cout << MAGENTA << "| " << RESET << BWHITE; cout.width(18); cout << col3; }
    if (!col4.empty()) { cout << MAGENTA << "| " << RESET << BWHITE; cout.width(10); cout << col4; }
    cout << MAGENTA << "|" << RESET << endl;

    cout << BOLD << MAGENTA;
    cout << "  +-----------------------+---------------------------+";
    if (!col3.empty()) cout << "-------------------+";
    if (!col4.empty()) cout << "-----------+";
    cout << RESET << endl;
}

void tableRow(const string& c1, const string& c2, const string& c3 = "", const string& c4 = "") {
    cout << "  " << CYAN << "| " << RESET
         << BCYAN << left;
    cout.width(22); cout << c1;
    cout << CYAN << "| " << RESET << BWHITE;
    cout.width(26); cout << c2;
    if (!c3.empty()) { cout << CYAN << "| " << RESET << BYELLOW; cout.width(18); cout << c3; }
    if (!c4.empty()) { cout << CYAN << "| " << RESET << BGREEN;  cout.width(10); cout << c4; }
    cout << CYAN << "|" << RESET << endl;
}

void tableFooter(bool col3 = false, bool col4 = false) {
    cout << CYAN;
    cout << "  +-----------------------+---------------------------+";
    if (col3) cout << "-------------------+";
    if (col4) cout << "-----------+";
    cout << RESET << endl;
}

// ============================================================
//  MODULE 1 — INITIALIZATION
// ============================================================
void initializeSystem() {
    printAsciiLogo();
    sectionHeader(1, "SYSTEM INITIALIZATION");
    sessionLogs.push("Session Started: Operation Logging Initialized.");

    char choice;
    prompt("Load existing data from 'RailwayData.txt'? (y/n): ");
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        ifstream inFile("RailwayData.txt");
        if (inFile.is_open()) {
            int trainID, coachCount;
            string trainName;
            while (inFile >> trainID >> trainName >> coachCount) {
                Train t(trainID, trainName);
                for (int i = 0; i < coachCount; i++) {
                    string coachType;
                    int seatCount;
                    if (!(inFile >> coachType >> seatCount)) break;
                    Coach c(coachType);
                    for (int j = 0; j < seatCount; j++) {
                        int sNum;
                        string pName;
                        if (!(inFile >> sNum >> pName)) break;
                        c.bookSeat(sNum, pName);
                    }
                    t.addCoach(c);
                }
                registry.insert(t);
                if (!inFile) break;
            }
            tagSuccess("Train registry loaded from RailwayData.txt");
            sessionLogs.push("Data loaded from file.");
        }

        ifstream netFile("NetworkData.txt");
        if (netFile.is_open()) {
            int vCount;
            netFile >> vCount;
            netFile.ignore();
            for (int i = 0; i < vCount; i++) {
                string name;
                getline(netFile, name);
                network.addStation(name);
            }
            int eCount;
            netFile >> eCount;
            for (int i = 0; i < eCount; i++) {
                int u, v, w;
                netFile >> u >> v >> w;
                network.addEdge(u, v, w);
            }
            netFile.close();
            tagSuccess("Network topology loaded from NetworkData.txt");
        }
        else {
            tagError("No network data file found. Starting with empty network.");
        }
    }
    else {
        tagInfo("Initializing empty registry and network.");
        sessionLogs.push("Initialized empty system.");
    }
    divider();
}

// ============================================================
//  MODULE 2 — FLEET MANAGEMENT
// ============================================================
void fleetManagement() {
    int subChoice;
    do {
        sectionHeader(2, "FLEET SETUP  |  TRAIN REGISTRY");
        menuItem(1, "Register New Train");
        menuItem(2, "Add Coach  (Front / Back / Index)");
        menuItem(3, "Visual Check  (Traverse Coaches)");
        menuItem(4, "Remove Coach from Train");
        menuItem(5, "De-register (Remove) Train");
        menuItem(6, "Back to Main Menu");
        divider();
        prompt("Choice: ");
        cin >> subChoice;

        if (subChoice == 1) {
            int id;
            string name;
            prompt("Train ID (unique integer): "); cin >> id;
            prompt("Train Name: ");                cin >> name;
            registry.insert(Train(id, name));
            string logMsg = "Registered Train [" + to_string(id) + "] " + name;
            sessionLogs.push(logMsg);
            undoStack.push("DEL_TRAIN|" + to_string(id));
            tagSuccess("Train [" + to_string(id) + "] " + name + " registered.");
        }
        else if (subChoice == 2) {
            int id;
            prompt("Train ID: "); cin >> id;
            Train* t = registry.search(Train(id, ""));
            if (t) {
                string cName;
                int posChoice, idx;
                prompt("Coach Name (e.g., Business-1): "); cin >> cName;
                cout << endl;
                menuItem(1, "Insert at Front");
                menuItem(2, "Insert at End");
                menuItem(3, "Insert at Specific Index");
                prompt("Position Choice: "); cin >> posChoice;

                int actualIndex = 0;
                if (posChoice == 1) {
                    t->addCoachAt(0, Coach(cName)); actualIndex = 0;
                }
                else if (posChoice == 2) {
                    t->addCoach(Coach(cName)); actualIndex = t->coaches.getSize() - 1;
                }
                else {
                    prompt("Index: "); cin >> idx;
                    t->addCoachAt(idx, Coach(cName));
                    actualIndex = idx;
                }
                undoStack.push("DEL_COACH|" + to_string(id) + "|" + to_string(actualIndex));
                sessionLogs.push("Added Coach " + cName + " to Train " + to_string(id));
                tagSuccess("Coach '" + cName + "' attached to Train " + to_string(id) + ".");
            }
            else tagError("Train ID " + to_string(id) + " not found in registry.");
        }
        else if (subChoice == 3) {
            int id;
            prompt("Train ID to Traverse: "); cin >> id;
            Train* t = registry.search(Train(id, ""));
            if (t) {
                phaseHeader("COACH SEQUENCE  |  " + t->name);
                cout << "  " << BCYAN;
                t->displayTrain();
                cout << RESET;
            }
            else tagError("Train ID " + to_string(id) + " not found.");
        }
        else if (subChoice == 4) {
            int id, idx;
            prompt("Train ID: "); cin >> id;
            Train* t = registry.search(Train(id, ""));
            if (t) {
                prompt("Coach Index to Remove: "); cin >> idx;
                Coach* c = t->getCoach(idx);
                string cName = c ? c->getType() : "Coach";
                t->removeCoach(idx);
                undoStack.push("ADD_COACH|" + to_string(id) + "|" + to_string(idx) + "|" + cName);
                tagWarn("Coach '" + cName + "' removed from Train " + to_string(id) + ".");
                sessionLogs.push("Removed coach at index " + to_string(idx) + " from Train " + to_string(id));
            }
            else tagError("Train ID " + to_string(id) + " not found.");
        }
        else if (subChoice == 5) {
            int id;
            prompt("Train ID to De-register: "); cin >> id;
            Train* t = registry.search(Train(id, ""));
            if (t) {
                undoStack.push("ADD_TRAIN|" + to_string(id) + "|" + t->name);
                registry.remove(Train(id, ""));
                sessionLogs.push("De-registered Train ID: " + to_string(id));
                tagWarn("Train " + to_string(id) + " removed from registry.");
            }
            else tagError("Train ID " + to_string(id) + " not found.");
        }
        else if (subChoice != 6) {
            tagError("Invalid option. Enter 1-6.");
        }
    } while (subChoice != 6);
}

// ============================================================
//  MODULE 3 — NETWORK MAPPING
// ============================================================
void networkMapping() {
    int subChoice;
    do {
        sectionHeader(3, "ROUTE MANAGEMENT  |  NETWORK TOPOLOGY");

        cout << "  " << DIM << CYAN << "-- Station Management --" << RESET << endl;
        menuItem(1, "Add New Station");
        menuItem(2, "Remove Station");
        menuItem(3, "Rename Station");
        cout << endl;
        cout << "  " << DIM << CYAN << "-- Track Management --" << RESET << endl;
        menuItem(4, "Add Route  (Connect Tracks)");
        menuItem(5, "Remove Route");
        cout << endl;
        cout << "  " << DIM << CYAN << "-- Optimization --" << RESET << endl;
        menuItem(6, "Dijkstra Shortest Path");
        menuItem(7, "Back to Main Menu");
        divider();
        prompt("Choice: ");
        cin >> subChoice;

        if (subChoice == 1) {
            string name;
            prompt("City / Station Name: "); cin >> name;
            network.addStation(name);
            int newId = network.getNumVertices() - 1;
            undoStack.push("DEL_STATION|" + to_string(newId));
            sessionLogs.push("Added Station: " + name);
            tagSuccess("Station '" + name + "' added with ID: " + to_string(newId));
        }
        else if (subChoice == 2) {
            int id;
            prompt("Hub ID to Remove: "); cin >> id;
            string oldName = network.getStationName(id);
            network.removeStation(id);
            undoStack.push("RESTORE_STATION|" + to_string(id) + "|" + oldName);
            sessionLogs.push("Removed Station ID: " + to_string(id));
            tagWarn("Station '" + oldName + "' (ID " + to_string(id) + ") decommissioned.");
        }
        else if (subChoice == 3) {
            int id; string name;
            prompt("Hub ID: ");            cin >> id;
            prompt("New Station Name: "); cin >> name;
            network.setStationName(id, name);
            sessionLogs.push("Renamed Station ID " + to_string(id) + " to " + name);
            tagSuccess("Station " + to_string(id) + " renamed to '" + name + "'.");
        }
        else if (subChoice == 4) {
            int u, v, w;
            prompt("Start Hub ID: ");  cin >> u;
            prompt("End Hub ID: ");    cin >> v;
            prompt("Distance (km): "); cin >> w;
            network.addEdge(u, v, w);
            undoStack.push("DEL_ROUTE|" + to_string(u) + "|" + to_string(v));
            sessionLogs.push("Connected Tracks: " + to_string(u) + " <-> " + to_string(v));
            tagSuccess("Route " + to_string(u) + " <-> " + to_string(v) + " (" + to_string(w) + "km) added.");
        }
        else if (subChoice == 5) {
            int u, v;
            prompt("Start Hub ID: "); cin >> u;
            prompt("End Hub ID: ");   cin >> v;
            int w = network.getWeight(u, v);
            network.removeEdge(u, v);
            if (w != -1) undoStack.push("ADD_ROUTE|" + to_string(u) + "|" + to_string(v) + "|" + to_string(w));
            sessionLogs.push("Removed Route: " + to_string(u) + " <-> " + to_string(v));
            tagWarn("Route " + to_string(u) + " <-> " + to_string(v) + " removed.");
        }
        else if (subChoice == 6) {
            int start, end;
            prompt("Start Hub ID: ");       cin >> start;
            prompt("Destination Hub ID: "); cin >> end;
            phaseHeader("DIJKSTRA  |  SHORTEST PATH ANALYSIS");
            cout << BCYAN << "  Calculating most efficient path..." << RESET << endl << endl;
            network.dijkstra(start, end);
        }
        else if (subChoice != 7) {
            tagError("Invalid option. Enter 1-7.");
        }
    } while (subChoice != 7);
}

// ============================================================
//  MODULE 4 — LIVE OPERATIONS
// ============================================================
void liveOperations() {
    int subChoice;
    do {
        sectionHeader(4, "LIVE OPERATIONS  |  SEAT MANAGEMENT");
        menuItem(1, "Seat Inquiry  (O(1) Hash Lookup)");
        menuItem(2, "Book Seat");
        menuItem(3, "Cancel Booking");
        menuItem(4, "Display Seating Chart  (BST Sorted)");
        menuItem(5, "Reverse Train Direction");
        menuItem(6, "Back to Main Menu");
        divider();
        prompt("Choice: ");
        cin >> subChoice;

        if (subChoice >= 1 && subChoice <= 4) {
            int tid;
            prompt("Train ID: "); cin >> tid;
            Train* t = registry.search(Train(tid, ""));
            if (t) {
                int cIdx;
                prompt("Coach Index (0-" + to_string(t->coaches.getSize() - 1) + "): ");
                cin >> cIdx;
                Coach* c = t->getCoach(cIdx);
                if (c) {
                    if (subChoice == 1) {
                        int sNum;
                        prompt("Seat Number: "); cin >> sNum;
                        phaseHeader("SEAT INQUIRY RESULT");
                        c->findPassenger(sNum);
                    }
                    else if (subChoice == 2) {
                        int sNum; string pName;
                        prompt("Seat Number: ");       cin >> sNum;
                        prompt("Passenger Name: ");    cin >> pName;
                        if (c->bookSeat(sNum, pName)) {
                            undoStack.push("CANCEL_SEAT|" + to_string(tid) + "|" + to_string(cIdx) + "|" + to_string(sNum));
                            tagSuccess("Seat " + to_string(sNum) + " booked for " + pName + ".");
                            sessionLogs.push("Booked Seat " + to_string(sNum) + " in " + c->getType());
                        }
                    }
                    else if (subChoice == 3) {
                        int sNum;
                        prompt("Seat Number to Cancel: "); cin >> sNum;
                        c->cancelSeat(sNum);
                        undoStack.push("MSG|Cannot fully undo seat cancellation without passenger name recovery.");
                        tagWarn("Booking for Seat " + to_string(sNum) + " cancelled.");
                        sessionLogs.push("Cancelled Seat " + to_string(sNum) + " in " + c->getType());
                    }
                    else if (subChoice == 4) {
                        phaseHeader("SEATING CHART  |  " + c->getType());
                        cout << "  " << BCYAN;
                        c->displaySortedSeats();
                        cout << RESET;
                    }
                }
                else tagError("Coach index out of range.");
            }
            else tagError("Train ID " + to_string(tid) + " not found.");
        }
        else if (subChoice == 5) {
            int tid;
            prompt("Train ID to Reverse: "); cin >> tid;
            Train* t = registry.search(Train(tid, ""));
            if (t) {
                t->reverseCoaches();
                undoStack.push("REVERSE_TRAIN|" + to_string(tid));
                tagSuccess("Train " + to_string(tid) + " orientation flipped for return journey.");
                sessionLogs.push("Reversed Train " + to_string(tid));
            }
            else tagError("Train ID " + to_string(tid) + " not found.");
        }
        else if (subChoice != 6) {
            tagError("Invalid option. Enter 1-6.");
        }
    } while (subChoice != 6);
}

// ============================================================
//  MODULE 5 — CRISIS MANAGEMENT & LOGS
// ============================================================
void crisisAndLogs() {
    int subChoice;
    do {
        sectionHeader(5, "CRISIS MANAGEMENT  |  AUDIT LOGS");
        menuItem(1, "Emergency Search  (AVL O(log n))");
        menuItem(2, "Log Review  (Session History)");
        menuItem(3, "Undo Last Action");
        menuItem(4, "Back to Main Menu");
        divider();
        prompt("Choice: ");
        cin >> subChoice;

        if (subChoice == 1) {
            int id;
            prompt("Train ID for Emergency Locate: "); cin >> id;
            Train* t = registry.search(Train(id, ""));
            phaseHeader("EMERGENCY SEARCH RESULT");
            if (t) {
                tableHeader("Train ID", "Name", "Status");
                tableRow(to_string(t->trainID), t->name, "ACTIVE / REACHABLE");
                tableFooter(true);
                tagLocated("Train " + t->name + " is active and reachable.");
            }
            else {
                tagError("No train with ID " + to_string(id) + " in registry.");
            }
        }
        else if (subChoice == 2) {
            phaseHeader("SESSION HISTORY  |  LIFO AUDIT TRAIL");
            if (sessionLogs.isEmpty()) {
                tagInfo("No logs recorded yet.");
            }
            else {
                Stack<string> tempStack = sessionLogs;
                int idx = 1;
                while (!tempStack.isEmpty()) {
                    cout << "  " << CYAN << "[" << BWHITE << idx++ << CYAN << "]"
                         << RESET << "  " << WHITE << tempStack.peek() << RESET << endl;
                    tempStack.pop();
                }
            }
        }
        else if (subChoice == 3) {
            if (!undoStack.isEmpty()) {
                string lastAction = undoStack.peek();
                phaseHeader("UNDO  |  REVERTING LAST ACTION");
                tagWarn("Reverting: " + lastAction);

                size_t pos1 = lastAction.find('|');
                string type      = lastAction.substr(0, pos1);
                string remaining = (pos1 == string::npos) ? "" : lastAction.substr(pos1 + 1);

                if (type == "DEL_TRAIN") {
                    int id = stoi(remaining);
                    registry.remove(Train(id, ""));
                    tagUndo("Train " + to_string(id) + " removed.");
                }
                else if (type == "ADD_TRAIN") {
                    size_t p = remaining.find('|');
                    int id    = stoi(remaining.substr(0, p));
                    string name = remaining.substr(p + 1);
                    registry.insert(Train(id, name));
                    tagUndo("Train " + to_string(id) + " (" + name + ") restored.");
                }
                else if (type == "DEL_COACH") {
                    size_t p = remaining.find('|');
                    int tid  = stoi(remaining.substr(0, p));
                    int idx  = stoi(remaining.substr(p + 1));
                    Train* t = registry.search(Train(tid, ""));
                    if (t) t->removeCoach(idx);
                    tagUndo("Coach removed from Train " + to_string(tid) + ".");
                }
                else if (type == "ADD_COACH") {
                    size_t p1   = remaining.find('|');
                    size_t p2   = remaining.find('|', p1 + 1);
                    int tid     = stoi(remaining.substr(0, p1));
                    int idx     = stoi(remaining.substr(p1 + 1, p2 - p1 - 1));
                    string name = remaining.substr(p2 + 1);
                    Train* t    = registry.search(Train(tid, ""));
                    if (t) t->addCoachAt(idx, Coach(name));
                    tagUndo("Coach '" + name + "' restored to Train " + to_string(tid) + ".");
                }
                else if (type == "DEL_STATION") {
                    int id = stoi(remaining);
                    network.removeStation(id);
                    tagUndo("Station " + to_string(id) + " decommissioned.");
                }
                else if (type == "RESTORE_STATION") {
                    size_t p    = remaining.find('|');
                    int id      = stoi(remaining.substr(0, p));
                    string name = remaining.substr(p + 1);
                    network.setStationName(id, name);
                    tagUndo("Station " + to_string(id) + " renamed back to '" + name + "'.");
                }
                else if (type == "DEL_ROUTE") {
                    size_t p = remaining.find('|');
                    int u    = stoi(remaining.substr(0, p));
                    int v    = stoi(remaining.substr(p + 1));
                    network.removeEdge(u, v);
                    tagUndo("Route " + to_string(u) + " <-> " + to_string(v) + " removed.");
                }
                else if (type == "ADD_ROUTE") {
                    size_t p1 = remaining.find('|');
                    size_t p2 = remaining.find('|', p1 + 1);
                    int u = stoi(remaining.substr(0, p1));
                    int v = stoi(remaining.substr(p1 + 1, p2 - p1 - 1));
                    int w = stoi(remaining.substr(p2 + 1));
                    network.addEdge(u, v, w);
                    tagUndo("Route " + to_string(u) + " <-> " + to_string(v) +
                            " (weight " + to_string(w) + "km) restored.");
                }
                else if (type == "CANCEL_SEAT") {
                    size_t p1 = remaining.find('|');
                    size_t p2 = remaining.find('|', p1 + 1);
                    int tid   = stoi(remaining.substr(0, p1));
                    int cIdx  = stoi(remaining.substr(p1 + 1, p2 - p1 - 1));
                    int sNum  = stoi(remaining.substr(p2 + 1));
                    Train* t  = registry.search(Train(tid, ""));
                    if (t) {
                        Coach* c = t->getCoach(cIdx);
                        if (c) c->cancelSeat(sNum);
                    }
                    tagUndo("Seat " + to_string(sNum) + " booking cancelled.");
                }
                else if (type == "REVERSE_TRAIN") {
                    int tid  = stoi(remaining);
                    Train* t = registry.search(Train(tid, ""));
                    if (t) t->reverseCoaches();
                    tagUndo("Train " + to_string(tid) + " orientation reversed back.");
                }
                else if (type == "MSG") {
                    tagInfo(remaining);
                }

                undoStack.pop();
                sessionLogs.push("Performed Undo: " + lastAction);
            }
            else {
                tagInfo("Nothing to undo.");
            }
        }
        else if (subChoice != 4) {
            tagError("Invalid option. Enter 1-4.");
        }
    } while (subChoice != 4);
}

// ============================================================
//  MODULE 6 — SAVE & EXIT
// ============================================================
void saveProgress() {
    sectionHeader(6, "SAVING PROGRESS");

    ofstream outFile("RailwayData.txt");
    if (outFile.is_open()) {
        registry.saveLevelOrder(outFile);
        outFile.close();
        tagSuccess("Train registry saved to RailwayData.txt");
    }
    else {
        tagError("Could not open RailwayData.txt for writing.");
    }

    ofstream netFile("NetworkData.txt");
    if (netFile.is_open()) {
        network.saveToFile(netFile);
        netFile.close();
        tagSuccess("Network topology saved to NetworkData.txt");
    }
    else {
        tagError("Could not open NetworkData.txt for writing.");
    }

    divider();
    cout << BOLD << BGREEN << "  System state preserved. Hierarchical structure intact." << RESET << endl;
}

// ============================================================
//  MAIN — COMMAND CENTER
// ============================================================
int main() {
    enableWindowsANSI();
    initializeSystem();

    int choice;
    do {
        sectionHeader(0, "THE IRON NEXUS  --  COMMAND CENTER");
        menuItem(1, "Fleet Management   (Train Registry)");
        menuItem(2, "Network Mapping    (Route Topology)");
        menuItem(3, "Live Operations    (Bookings & Seats)");
        menuItem(4, "Crisis & Logs      (Security & Undo)");
        menuItem(5, "Save & Exit");
        divider();
        prompt("Choice: ");

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            tagError("Invalid input. Please enter a number.");
            continue;
        }

        switch (choice) {
            case 1: fleetManagement(); break;
            case 2: networkMapping();  break;
            case 3: liveOperations();  break;
            case 4: crisisAndLogs();   break;
            case 5: saveProgress();    break;
            default: tagError("Invalid option. Enter 1-5."); break;
        }
    } while (choice != 5);

    divider();
    cout << BOLD << BMAGENTA
         << "\n  Closing Iron Nexus Command Center. Safe journeys!\n"
         << RESET << endl;
    return 0;
}