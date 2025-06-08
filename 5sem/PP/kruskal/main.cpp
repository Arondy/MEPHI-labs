#include <iostream>
#include <algorithm>
#include <functional>
#include <random>
#include <vector>
#include <set>
#include <omp.h>

#define MIN_WEIGHT 1
#define MAX_WEIGHT (VERTICES_NUM * 10)
#define VERTICES_NUM 1e3
#define EDGES_NUM (VERTICES_NUM * (VERTICES_NUM - 1) / 2)

#define SEED 920215
#define SEED_INC 123456
#define RUNS_NUM 10

using std::vector, std::set, std::pair, std::swap, std::min, std::max, std::cout, std::cin, std::endl;

class Edge {
public:
    int u;
    int v;
    int weight;

    Edge() = default;

    Edge(const int u, const int v, const int weight): u{u}, v{v}, weight{weight}{}

    bool operator<(Edge const &other) const{
        return weight < other.weight;
    }

    friend std::ostream &operator<<(std::ostream &os, const Edge &edge){
        os << edge.u << " - " << edge.v << " (" << edge.weight << ")";
        return os;
    }

    bool operator==(Edge const &other) const{
        return u == other.u && v == other.v;
    }
};

// set - множество, {}
void make_set(const int &v, vector<int> &parent, vector<int> &rank){
    parent[v] = v;
    rank[v] = 0;
}

int find_set_representative(const int &v, vector<int> &parent, vector<int> &rank){
    if (v == parent[v])
        return v;

    parent[v] = find_set_representative(parent[v], parent, rank);

    return parent[v];
}

void union_sets(int a, int b, vector<int> &parent, vector<int> &rank){
    a = find_set_representative(a, parent, rank);
    b = find_set_representative(b, parent, rank);

    if (a != b){
        if (rank[a] < rank[b]){
            swap(a, b);
        }
        parent[b] = a;
        if (rank[a] == rank[b]){
            rank[a]++;
        }
    }
}

// V - количество вершин, E - количество ребер
vector<Edge> generate_random_connected_graph(const int &V, const int &E, const int &seed){
    if (E < V - 1){
        throw std::invalid_argument("It is impossible to create a connected graph: there are too few edges!");
    }
    if (E > V * (V - 1) / 2){
        throw std::invalid_argument("It is impossible to create a graph: there are too many edges!");
    }
    const double start = omp_get_wtime();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> weight_dist(MIN_WEIGHT, MAX_WEIGHT);
    vector<Edge> edges;
    vector<int> vertices(V);

    if constexpr (EDGES_NUM < VERTICES_NUM * (VERTICES_NUM - 1) / 2 - (VERTICES_NUM - 1)){
        if constexpr (VERTICES_NUM >= 5e3){
            // cout << "You shouldn't have come here... It was better to generate a complete graph..." << endl;
        }

        for (int i = 0; i < V; ++i){
            vertices[i] = i;
        }

        // Перемешиваем вершины для случайного соединения
        std::shuffle(vertices.begin(), vertices.end(), gen);

        // Создаём V-1 рёбер для остовного дерева
        for (int i = 1; i < V; ++i){
            int u = vertices[i - 1];
            int v = vertices[i];
            int weight = weight_dist(gen);

            if (u < v){
                edges.emplace_back(u, v, weight);
            } else{
                edges.emplace_back(v, u, weight);
            }
        }

        if (edges.size() == E){
            return edges;
        }
    }

    // Генерация всех возможных рёбер
    vector<Edge> all_edges(VERTICES_NUM * (VERTICES_NUM - 1) / 2);
    for (int u = 0; u < V; ++u){
        for (int v = u + 1; v < V; ++v){
            int weight = weight_dist(gen);
            int index = u * (V - 1) - u * (u + 1) / 2 + v - u - 1;
            all_edges[index] = Edge{u, v, weight};
        }
    }

    if constexpr (EDGES_NUM < VERTICES_NUM * (VERTICES_NUM - 1) / 2 - (VERTICES_NUM - 1)){
        // Удаление остовных ребер
        omp_lock_t lock;
        omp_init_lock(&lock);
#pragma omp parallel for num_threads(omp_get_num_procs())
        for (const Edge &edge: edges){
            if (auto it = std::find(all_edges.begin(), all_edges.end(), edge); it != all_edges.end()){
                omp_set_lock(&lock);
                all_edges.erase(it);
                omp_unset_lock(&lock);
            }
        }
    }

    // Перемешиваем все рёбра
    std::shuffle(all_edges.begin(), all_edges.end(), gen);

    // Копируем нужное количество рёбер
    int needed_edges = E - edges.size();
    edges.insert(edges.end(), all_edges.begin(), all_edges.begin() + needed_edges);
    const double end = omp_get_wtime();
    // cout << "Generation took " << end - start << " seconds for " << edges.size() << " edges" << endl;

    return edges;
}

vector<Edge> findMST_Kruskal(vector<Edge> &edges){
    vector<int> parent(EDGES_NUM);
    vector<int> rank(EDGES_NUM);
    vector<Edge> result;

    sort(edges.begin(), edges.end());

    // Создаем подмножество из каждой вершины
    for (int i = 0; i < EDGES_NUM; i++){
        make_set(i, parent, rank);
    }

    for (Edge e: edges){
        if (find_set_representative(e.u, parent, rank) != find_set_representative(e.v, parent, rank)){
            result.push_back(e);
            union_sets(e.u, e.v, parent, rank);
        }
    }

    return result;
}

// Чтобы можно было запихнуть в time_algorithm
// Не перегрузка, чтобы передать без static_cast
vector<Edge> findMST_Kruskal_t(vector<Edge> &edges, const int &_){
    return findMST_Kruskal(edges);
}

// Функция для слияния двух отсортированных массивов
vector<Edge> merge(const vector<Edge> &a, const vector<Edge> &b){
    vector<Edge> merged;
    size_t i = 0, j = 0;

    while (i < a.size() && j < b.size()){
        if (a[i] < b[j]){
            merged.push_back(a[i++]);
        } else{
            merged.push_back(b[j++]);
        }
    }

    while (i < a.size()){
        merged.push_back(a[i++]);
    }

    while (j < b.size()){
        merged.push_back(b[j++]);
    }

    return merged;
}

vector<Edge> merge_sort(const vector<vector<Edge>> &local_edge_sets, int left, int right){
    if (left == right){
        return local_edge_sets[left];
    }

    int mid = left + (right - left) / 2;
    vector<Edge> left_sorted = merge_sort(local_edge_sets, left, mid);
    vector<Edge> right_sorted = merge_sort(local_edge_sets, mid + 1, right);

    return merge(left_sorted, right_sorted);
}

vector<Edge> findMST_ParallelKruskal(vector<Edge> &edges, const int &threads_num){
    vector<Edge> result;

    int edges_per_thread = edges.size() / threads_num;
    vector<vector<Edge>> local_edge_sets(threads_num);
#pragma omp parallel num_threads(threads_num)
    {
        int thread_id = omp_get_thread_num();
        int start_id = thread_id * edges_per_thread;
        int end_id = (thread_id == threads_num - 1) ? edges.size() : start_id + edges_per_thread;

        vector<Edge> local_edges(edges.begin() + start_id, edges.begin() + end_id);
        // sort(local_edges.begin(), local_edges.end());
        local_edge_sets[thread_id] = findMST_Kruskal(local_edges);
    }
    // Слияние всех локальных MST в один массив с использованием merge_sort
    vector<Edge> merged_edges = merge_sort(local_edge_sets, 0, threads_num - 1);

    // Находим MST для всего множества рёбер с использованием Kruskal
    result = findMST_Kruskal(merged_edges);

    return result;
}

void time_algorithm(const int &min_threads_num, const int &max_threads_num,
                    std::function<vector<Edge>(vector<Edge> &, const int &)> func){
    for (int t = min_threads_num; t <= max_threads_num; t++){
        double time_spent = 0;
        int seed = SEED;

        for (int i = 0; i < RUNS_NUM; i++){
            vector<Edge> edges = generate_random_connected_graph(VERTICES_NUM, EDGES_NUM, seed);
            seed += SEED_INC;

            const double start = omp_get_wtime();
            func(edges, t);
            const double end = omp_get_wtime();

            time_spent += end - start;
        }
        cout << time_spent / RUNS_NUM << endl;
    }
}

int main(){
    // cout << "Consecutive Kruskal:" << endl;
    // time_algorithm(1, 1, findMST_Kruskal_t);

    cout << "Parallel Kruskal:" << endl;
    time_algorithm(6, 6, findMST_ParallelKruskal);
}
