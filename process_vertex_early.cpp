//поиск шарниров
#include <iostream>
#include <vector>
#include <string>
#include<set>
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
    vector<int> reachable_ancestor;  //хранит вершину с минимальным entry_time (самую близкую к началу)
    vector<int> children; // Счетчик прямых потомков в дереве DFS
    set<int> sharnir; //Сет(уникальные элементы) для хранения уникальных шарниров
    int time_counter{0};

    
     void dfs_v(int u) {
        sost[u] = versh_sost::Discovered;
        entry_time[u] = ++time_counter;
        reachable_ancestor[u] = u; 
        children[u] = 0;

        for (int i = 0; i < smezh_spis[u].size(); ++i) {
            int v = smezh_spis[u][i].to;

            if (sost[v] == versh_sost::Undiscovered) {
                children[u]++; 
                parent[v] = u;
                cout << "  Древесное ребро: " << u << " -> " << v << "\n";
                
                dfs_v(v);
                // ПОСЛЕ ВОЗВРАТА: u перенимает лучший адрес куда может попасть от своего потомка v
                if (entry_time[reachable_ancestor[v]] < entry_time[reachable_ancestor[u]]) {
                    reachable_ancestor[u] = reachable_ancestor[v];
                }
                // Если u — не корень, и её ребенок v не смог прыгнуть выше самой u, то u - шарнир
                if (parent[u] != -1 and entry_time[reachable_ancestor[v]] >= entry_time[u]) {
                    sharnir.insert(u);
                }
            } 
            // Если это обратное ребро к предку (и не к прямому родителю в неориентированном графе)
            else if (sost[v] == versh_sost::Discovered and parent[u] != v) {
                cout << "  Найдено обратное ребро (цикл): " << u << " -> " << v << "\n";
                 // Обновляем reachable_ancestor напрямую по времени входа в соседа
                if (entry_time[v] < entry_time[reachable_ancestor[u]]) {
                    reachable_ancestor[u] = v;
                }
            }
        }
        //Проверка для корней dfs, корень = шарнир, если больше 1 потомка
        if (parent[u] == -1 and children[u] > 1) {
            sharnir.insert(u);
        }
        
        exit_time[u] = ++time_counter; 
        sost[u] = versh_sost::Processed;
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
        sost.assign(num_versh + 1, versh_sost::Undiscovered);
        parent.assign(num_versh + 1, -1);
        entry_time.assign(num_versh + 1, 0);
        exit_time.assign(num_versh + 1, 0);
        reachable_ancestor.assign(num_versh + 1, 0);
        children.assign(num_versh + 1, 0);
        sharnir.clear(); 
        
        time_counter = 0;
        dfs_v(start);
        
        cout<<"\nТочки шарнира ";
        for(auto i=sharnir.begin();i!=sharnir.end();i++){
            cout<< *i <<" ";
        }
    }
};

int main() {
    // Создаем неориентированный граф на 11 вершин
    Graph g(11, false);

    // Добавляем ребра
    g.add_edge(1,2);
    g.add_edge(2,3);
    g.add_edge(3,1); 
    g.add_edge(3,4);
    g.add_edge(4,5);
    g.add_edge(5,6);
    g.add_edge(2,5);
    g.add_edge(5,7);
    g.add_edge(6,7);
    g.add_edge(1,8);
    g.add_edge(8,9);
    g.add_edge(10,9);
    g.add_edge(11,10);
    g.add_edge(11,9);
    g.dfs(1);

    return 0;
}