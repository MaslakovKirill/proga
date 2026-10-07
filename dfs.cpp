#include <iostream>
#include <vector>
#include <string>
using namespace std;
enum class versh_sost {
    Undiscovered,
    Discovered,
    Processed
};

class Edge {
    public:
    int to; //номер вершины, в которую идет ребро
    int weight;
};

class Graph {
private:
    int num_versh;
    bool directed;
    vector<std::vector<Edge>> smezh_spis; 
    vector<versh_sost> sost;
    vector<int> parent;
    vector<int> entry_time;
    vector<int> exit_time;
    int time_counter{0};

    
    void dfs_rekurs(int u) {  //рекурсивная функция обхода
        sost[u] = versh_sost::Discovered;
        entry_time[u] = ++time_counter;

        cout << "Вход в вершину " << u << " время " << entry_time[u] << "\n";

       for (int i = 0; i < smezh_spis[u].size(); ++i) {
           int v = smezh_spis[u][i].to;

            if (sost[v] == versh_sost::Undiscovered) {
                parent[v] = u;
                cout << "  Древесное ребро: " << u << " -> " << v << "\n";
                dfs_rekurs(v);
            } 
            else if (sost[v] == versh_sost::Discovered && parent[u] != v) {
                cout << "  Найдено обратное ребро (цикл): " << u << " -> " << v << "\n";
            }
        }
    }

public:
    Graph(int vertices, bool directed) 
        : num_versh(vertices), directed(directed), smezh_spis(vertices + 1) {}

    void add_edge(int from, int to, int weight = 0) {
        smezh_spis[from].push_back({to, weight});
        if (!directed) {
            smezh_spis[to].push_back({from, weight});
        }
    }
    void dfs(int start) {
        //Неизменные характеристики графа
        sost.assign(num_versh + 1, versh_sost::Undiscovered); //состояние каждой вершины
        parent.assign(num_versh + 1, -1);//соответствие каждой вершины к "вершине-родителю"
        entry_time.assign(num_versh + 1, 0);//время входа в вершину
        exit_time.assign(num_versh + 1, 0);//время выхода из вершиеы
        time_counter = 0;

        dfs_rekurs(start);
    }
};

int main() {
    // Создаем неориентированный граф на 4 вершины
    Graph g(4, false);

    // Добавляем ребра (создаем цикл 1-2-3 и отдельное ребро к 4)
    g.add_edge(1, 2);
    g.add_edge(2, 1);
    g.add_edge(2, 3); 
    g.add_edge(3, 4);

    g.dfs(1);

    return 0;
}