#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include "MinHeap.h"
using namespace std;

struct Edge {
    int to;
    int weight;
    Edge* next;

    Edge(int t, int w, Edge* n) : to(t), weight(w), next(n) {}
};

class Graph {
private:
    int numVertices;
    Edge** adjList;
     string* stationNames;

public:
    Graph(int vertices) : numVertices(vertices) {
        adjList = new Edge * [numVertices];
        stationNames = new  string[numVertices];
        for (int i = 0; i < numVertices; i++) {
            adjList[i] = nullptr;
            stationNames[i] = "Station " +  to_string(i);
        }
    }

    ~Graph() {
        for (int i = 0; i < numVertices; i++) {
            Edge* current = adjList[i];
            while (current) {
                Edge* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] adjList;
        delete[] stationNames;
    }

    void setStationName(int id, const  string& name) {
        if (id >= 0 && id < numVertices) {
            stationNames[id] = name;
        }
    }

     string getStationName(int id) const {
        if (id >= 0 && id < numVertices) return stationNames[id];
        return "";
    }

    int getNumVertices() const { return numVertices; }

    int getWeight(int u, int v) const {
        Edge* current = adjList[u];
        while (current) {
            if (current->to == v) return current->weight;
            current = current->next;
        }
        return -1;
    }

    // Phase 4 Requirement: Add Station (Dynamic resizing)
    void addStation(const  string& name) {
        int newNumVertices = numVertices + 1;
        Edge** newAdjList = new Edge * [newNumVertices];
         string* newStationNames = new  string[newNumVertices];

        for (int i = 0; i < numVertices; i++) {
            newAdjList[i] = adjList[i];
            newStationNames[i] = stationNames[i];
        }

        newAdjList[numVertices] = nullptr;
        newStationNames[numVertices] = name;

        delete[] adjList;
        delete[] stationNames;

        adjList = newAdjList;
        stationNames = newStationNames;
        numVertices = newNumVertices;

         cout << "[SUCCESS] Station '" << name << "' added with ID: " << numVertices - 1 <<  endl;
    }

    void addEdge(int u, int v, int weight) {
        adjList[u] = new Edge(v, weight, adjList[u]);
        adjList[v] = new Edge(u, weight, adjList[v]); // Undirected graph for railway
    }

    void dijkstra(int startNode, int endNode) {
        int* dist = new int[numVertices];
        int* parent = new int[numVertices];
        bool* visited = new bool[numVertices];

        for (int i = 0; i < numVertices; i++) {
            dist[i] = 2147483647; // Infinity
            parent[i] = -1;
            visited[i] = false;
        }

        MinHeap<int> pq(numVertices * numVertices);
        dist[startNode] = 0;
        pq.push(startNode, 0);

        while (!pq.isEmpty()) {
            int u = pq.pop().vertex;

            if (visited[u]) continue;
            visited[u] = true;

            if (u == endNode) break;

            Edge* current = adjList[u];
            while (current) {
                int v = current->to;
                int weight = current->weight;

                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push(v, dist[v]);
                }
                current = current->next;
            }
        }

        printPath(parent, dist, startNode, endNode);

        delete[] dist;
        delete[] parent;
        delete[] visited;
    }

    void printPath(int* parent, int* dist, int startNode, int endNode) {
        if (dist[endNode] == 2147483647) {
             cout << "No path found from " << stationNames[startNode] << " to " << stationNames[endNode] <<  endl;
            return;
        }

         cout << "Most Efficient Route: ";
        displayRecursive(parent, endNode);
         cout << " [Total Distance: " << dist[endNode] << "km]" <<  endl;
    }

    void displayRecursive(int* parent, int current) {
        if (parent[current] == -1) {
             cout << stationNames[current];
            return;
        }
        displayRecursive(parent, parent[current]);
         cout << " -> " << stationNames[current];
    }

    // Rubric Requirement: Remove Route (Edge)
    void removeEdge(int u, int v) {
        auto removeSingleEdge = [&](int from, int to) {
            Edge* current = adjList[from];
            Edge* prev = nullptr;
            while (current) {
                if (current->to == to) {
                    if (prev) prev->next = current->next;
                    else adjList[from] = current->next;
                    delete current;
                    return true;
                }
                prev = current;
                current = current->next;
            }
            return false;
            };

        bool removed1 = removeSingleEdge(u, v);
        bool removed2 = removeSingleEdge(v, u);

        if (removed1 || removed2) {
             cout << "[SUCCESS] Route between " << stationNames[u] << " and " << stationNames[v] << " removed." <<  endl;
        }
        else {
             cout << "[ERROR] No route found between these stations." <<  endl;
        }
    }

    // Rubric Requirement: Remove Station (Vertex)
    // Note: In this fixed-size array implementation, "removing" a station 
    // means clearing its edges and resetting its name/status.
    void removeStation(int id) {
        if (id < 0 || id >= numVertices) return;

        // 1. Remove all outgoing edges from this station
        Edge* current = adjList[id];
        while (current) {
            Edge* next = current->next;
            // Also need to remove the back-link from the destination station
            int neighbor = current->to;

            // Remove edge from neighbor's list pointing back to 'id'
            Edge* nCurr = adjList[neighbor];
            Edge* nPrev = nullptr;
            while (nCurr) {
                if (nCurr->to == id) {
                    if (nPrev) nPrev->next = nCurr->next;
                    else adjList[neighbor] = nCurr->next;
                    delete nCurr;
                    break;
                }
                nPrev = nCurr;
                nCurr = nCurr->next;
            }

            delete current;
            current = next;
        }
        adjList[id] = nullptr;

         string oldName = stationNames[id];
        stationNames[id] = "DECOMMISSIONED";

         cout << "[SUCCESS] Station " << oldName << " (ID: " << id << ") has been removed and all connections severed." <<  endl;
    }

    void saveToFile(ofstream& outFile) {
        outFile << numVertices << "\n";
        for (int i = 0; i < numVertices; i++) {
            outFile << stationNames[i] << "\n";
        }
        int edgeCount = 0;
        for (int i = 0; i < numVertices; i++) {
            Edge* e = adjList[i];
            while (e) { if (i < e->to) edgeCount++; e = e->next; }
        }
        outFile << edgeCount << "\n";
        for (int i = 0; i < numVertices; i++) {
            Edge* e = adjList[i];
            while (e) {
                if (i < e->to)
                    outFile << i << " " << e->to << " " << e->weight << "\n";
                e = e->next;
            }
        }
    }
};

#endif
