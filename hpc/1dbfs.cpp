#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Tree {

    int V;
    vector<vector<int>> adj;

public:

    // Constructor
    Tree(int V) {

        this->V = V;
        adj.resize(V);
    }

    // Add Edge
    void addEdge(int parent, int child) {

        adj[parent].push_back(child);
        adj[child].push_back(parent); // Undirected Tree
    }

    // =====================================
    // Display Tree
    // =====================================
    void displayTree() {

        cout << "\nTree Structure:\n";

        for (int i = 0; i < V; i++) {

            cout << i << " -> ";

            for (int child : adj[i]) {

                cout << child << " ";
            }

            cout << endl;
        }
    }

    // =====================================
    // Parallel BFS
    // =====================================
    void parallelBFS(int start) {

        vector<bool> visited(V, false);

        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "\nParallel BFS Traversal: ";

        while (!q.empty()) {

            int node;

            #pragma omp critical
            {
                node = q.front();
                q.pop();

                cout << node << " ";
            }

            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++) {

                int neighbor = adj[node][i];

                if (!visited[neighbor]) {

                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {

                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }

        cout << endl;
    }

    // =====================================
    // Parallel DFS Utility
    // =====================================
    void parallelDFSUtil(int node, vector<bool>& visited) {

        bool alreadyVisited;

        #pragma omp critical
        {
            alreadyVisited = visited[node];

            if (!visited[node]) {

                visited[node] = true;

                cout << node << " ";
            }
        }

        if (alreadyVisited)
            return;

        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {

            int neighbor = adj[node][i];

            if (!visited[neighbor]) {

                parallelDFSUtil(neighbor, visited);
            }
        }
    }

    // =====================================
    // Parallel DFS
    // =====================================
    void parallelDFS(int start) {

        vector<bool> visited(V, false);

        cout << "\nParallel DFS Traversal: ";

        parallelDFSUtil(start, visited);

        cout << endl;
    }
};

// =====================================
// Main Function
// =====================================
int main() {

    int V;

    cout << "Enter number of nodes in tree: ";
    cin >> V;

    Tree t(V);

    cout << "Enter " << V - 1 << " edges:\n";

    for (int i = 0; i < V - 1; i++) {

        int u, v;

        cin >> u >> v;

        t.addEdge(u, v);
    }

    int root;

    cout << "Enter root node: ";
    cin >> root;

    // Display Tree
    t.displayTree();

    // BFS
    t.parallelBFS(root);

    // DFS
    t.parallelDFS(root);

    return 0;
}

// Enter number of nodes in tree: 6
// Enter 5 edges:
// 0 1
// 0 2
// 1 3
// 1 4
// 2 5
// Enter root node: 0

// Tree Structure:
// 0 -> 1 2 
// 1 -> 0 3 4 
// 2 -> 0 5 
// 3 -> 1 
// 4 -> 1 
// 5 -> 2 