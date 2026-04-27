#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <boost/graph/adjacency_matrix.hpp>

using Graph = boost::adjacency_matrix<boost::undirectedS,
                                      boost::no_property,
                                      boost::property<boost::edge_weight_t, int>>;

int main() {
    const int vertex_count = 10;

    Graph graph(vertex_count);

    auto weight_map = boost::get(boost::edge_weight, graph);

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    // Заполнение графа случайными весами
    for (int i = 0; i < vertex_count; ++i) {
        for (int j = i + 1; j < vertex_count; ++j) {
            auto edge = boost::add_edge(i, j, graph).first;
            weight_map[edge] = dist(engine);
        }
    }

    // Вывод матрицы смежности
    std::cout << "Adjacency matrix:\n";
    for (int i = 0; i < vertex_count; ++i) {
        for (int j = 0; j < vertex_count; ++j) {
            if (i == j) {
                std::cout << 0 << ' ';
            } else {
                auto edge = boost::edge(i, j, graph);
                std::cout << weight_map[edge.first] << ' ';
            }
        }
        std::cout << '\n';
    }

    std::vector<int> vertices;
    for (int i = 1; i < vertex_count; ++i) {
        vertices.push_back(i);
    }

    int min_cost = std::numeric_limits<int>::max();
    std::vector<int> best_path;

    do {
        int current_cost = 0;
        int current = 0;

        for (int v : vertices) {
            current_cost += weight_map[boost::edge(current, v, graph).first];
            current = v;
        }

        current_cost += weight_map[boost::edge(current, 0, graph).first];

        if (current_cost < min_cost) {
            min_cost = current_cost;
            best_path = {0};
            best_path.insert(best_path.end(), vertices.begin(), vertices.end());
            best_path.push_back(0);
        }

    } while (std::next_permutation(vertices.begin(), vertices.end()));

    std::cout << "\nOptimal path:\n";
    for (int v : best_path) {
        std::cout << v << ' ';
    }

    std::cout << "\nTotal cost: " << min_cost << '\n';

    return 0;
}