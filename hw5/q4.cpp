#include <bits/stdc++.h>
using namespace std;
int find(int x, map<int,int>& parents) {
    int px = parents[x];
    if (px != x)
        return find(px, parents);
    return px;
}
void combine(int x, int y, map<int,int>& parents) {
    int px = find(x,parents);
    int py = find(y,parents);
    parents[px] = py;
    parents[x] = py;
    parents[y] = py;
}
using edge_list = vector<tuple<int,int,int>>;
int kruskal(edge_list& edges, int num_vertices) {
    map<int,int> vertex_sets{};
    // edge_list& min_tree_edges{};
    int min_tree_weight = 0;
    for (int v = 0; v < num_vertices; ++v)
        vertex_sets.insert({v,v});
    int DS = num_vertices;
    int E = edges.size();
    while (DS > 1 && E > 0) {
        auto& [w,v1,v2] = edges[E-1];
        E -= 1;
        if (find(v1, vertex_sets) != find(v2, vertex_sets)) {
            min_tree_weight += w;
            combine(v1,v2,vertex_sets);
            DS -= 1;
        }
    }
    return min_tree_weight;
}

int main() {
    int N,M;
    cin >> N >> M;
    edge_list edges{};
    for (int i = 0; i < M; ++i) {
        int v1,v2,w;
        cin >> v1 >> v2 >> w;
        edges.emplace_back(w,v1,v2);
    }
    sort(edges.rbegin(),edges.rend());
    cout << kruskal(edges,N) << endl;
}
