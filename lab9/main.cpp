#include <iostream>
#include <vector>
#include <limits>

struct Edge {
    size_t u, v;
    int weight;
};

long long bellmanFord(size_t n, size_t m, size_t start, size_t finish, std::vector<Edge>& edges) {
    std::vector<long long> distance(n + 1, std::numeric_limits<long long>::max());
    distance[start] = 0;
    std::vector<int> parent(n + 1, -1);
    
    bool updated;

    for (size_t i = 1; i < n; ++i) {
        updated = false;
        
        for (size_t j = 0; j < m; ++j) {
            size_t u = edges[j].u;
            size_t v = edges[j].v;
            int weight = edges[j].weight;

            if (distance[u] != std::numeric_limits<long long>::max() && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                parent[v] = u;
                updated = true;
            }
        }

        if (!updated) {
            std::cout << "flag\n";
            break;
        }
    }

    if (distance[finish] == std::numeric_limits<long long>::max()) {
        return -1;
    }

    return distance[finish];
}

int main() {
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);

    size_t n, m, start, finish;
    std::cin >> n >> m >> start >> finish;

    std::vector<Edge> edges(m);
    for (size_t i = 0; i < m; ++i) {
        std::cin >> edges[i].u >> edges[i].v >> edges[i].weight;
    }

    long long result = bellmanFord(n, m, start, finish, edges);

	if (result == -1) {
    	std::cout << "No solution\n";
	}
	else {
		std::cout << result << '\n';
	}
    return 0;
}