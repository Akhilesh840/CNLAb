
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Edge {
    int u, v, weight;
};

int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    vector<Edge> edges(E);

    cout << "Enter edges (source destination weight):\n";
    for (int i = 0; i < E; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;
    }

    int source;
    cout << "Enter source vertex: ";
    cin >> source;

    vector<long long> dist(V, LLONG_MAX);
    dist[source] = 0;

    // Relax all edges V - 1 times
    for (int i = 0; i < V - 1; i++) {
        bool updated = false;

        for (const Edge &e : edges) {
            if (dist[e.u] != LLONG_MAX &&
                dist[e.u] + e.weight < dist[e.v]) {

                dist[e.v] = dist[e.u] + e.weight;
                updated = true;
            }
        }

        // Stop early if no distance changed
        if (!updated)
            break;
    }

    // Detect a reachable negative-weight cycle
    for (const Edge &e : edges) {
        if (dist[e.u] != LLONG_MAX &&
            dist[e.u] + e.weight < dist[e.v]) {
            cout << "Negative-weight cycle detected\n";
            return 0;
        }
    }

    cout << "\nShortest distances from vertex "
         << source << ":\n";

    for (int i = 0; i < V; i++) {
        cout << source << " -> " << i << " : ";

        if (dist[i] == LLONG_MAX)
            cout << "Unreachable";
        else
            cout << dist[i];

        cout << '\n';
    }

    return 0;
}
