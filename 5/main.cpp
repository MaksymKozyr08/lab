#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <vector>

using namespace std;

const int INF = 99999999; // big number for infinity

struct Edge {
  int to;
  int weight;
};

// comparator for sorting edges
bool cmpEdges(Edge a, Edge b) { return a.weight < b.weight; }

bool cmpEdgesDesc(Edge a, Edge b) { return a.weight > b.weight; }

class IGraph {
protected:
  int n; // vertices count
  bool is_dir;

public:
  IGraph(int v, bool directed = false) {
    n = v;
    is_dir = directed;
  }
  virtual ~IGraph() {}

  virtual void addEdge(int from, int to, int weight = 1) = 0;
  virtual vector<Edge> getNeighbors(int v) = 0;

  int getNumVertices() { return n; }
  bool getIsDirected() { return is_dir; }

  virtual int getWeight(int from, int to) = 0;

  void visualize() {
    ofstream out("graph_data.txt");
    out << n << " " << is_dir << endl;
    for (int i = 0; i < n; ++i) {
      vector<Edge> neighbors = getNeighbors(i);
      for (int j = 0; j < neighbors.size(); j++) {
        if (is_dir || i <= neighbors[j].to) {
          out << i << " " << neighbors[j].to << " " << neighbors[j].weight
              << endl;
        }
      }
    }
    out.close();

    // generate python script
    ofstream py("visualize.py");
    py << "import networkx as nx\n"
       << "import matplotlib.pyplot as plt\n\n"
       << "with open('graph_data.txt', 'r') as f:\n"
       << "    lines = f.readlines()\n"
       << "    v_count, is_directed = map(int, lines[0].split())\n"
       << "    if is_directed:\n"
       << "        G = nx.DiGraph()\n"
       << "    else:\n"
       << "        G = nx.Graph()\n"
       << "    G.add_nodes_from(range(v_count))\n"
       << "    for line in lines[1:]:\n"
       << "        u, v, w = map(int, line.split())\n"
       << "        G.add_edge(u, v, weight=w)\n\n"
       << "pos = nx.spring_layout(G)\n"
       << "nx.draw(G, pos, with_labels=True, node_color='skyblue', "
          "node_size=600)\n"
       << "labels = nx.get_edge_attributes(G, 'weight')\n"
       << "nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)\n"
       << "plt.show()\n";
    py.close();

    system("py visualize.py");
  }
};

class GraphMatrix : public IGraph {
  vector<vector<int>> matrix;

public:
  GraphMatrix(int v, bool directed = false) : IGraph(v, directed) {
    matrix.resize(v, vector<int>(v, INF));
    for (int i = 0; i < v; i++) {
      matrix[i][i] = 0;
    }
  }

  void addEdge(int from, int to, int weight = 1) {
    matrix[from][to] = weight;
    if (!is_dir) {
      matrix[to][from] = weight;
    }
  }

  vector<Edge> getNeighbors(int v) {
    vector<Edge> res;
    for (int i = 0; i < n; i++) {
      if (i != v && matrix[v][i] != INF) {
        Edge e;
        e.to = i;
        e.weight = matrix[v][i];
        res.push_back(e);
      }
    }
    return res;
  }

  int getWeight(int from, int to) { return matrix[from][to]; }
};

class GraphList : public IGraph {
  vector<vector<Edge>> adj;

public:
  GraphList(int v, bool directed = false) : IGraph(v, directed) {
    adj.resize(v);
  }

  void addEdge(int from, int to, int weight = 1) {
    Edge e;
    e.to = to;
    e.weight = weight;
    adj[from].push_back(e);
    if (!is_dir && from != to) {
      Edge e2;
      e2.to = from;
      e2.weight = weight;
      adj[to].push_back(e2);
    }
  }

  vector<Edge> getNeighbors(int v) { return adj[v]; }

  int getWeight(int from, int to) {
    if (from == to)
      return 0;
    for (int i = 0; i < adj[from].size(); i++) {
      if (adj[from][i].to == to)
        return adj[from][i].weight;
    }
    return INF;
  }
};

// converting
GraphList convertMatrixToList(GraphMatrix *gm) {
  int v = gm->getNumVertices();
  GraphList gl(v, gm->getIsDirected());
  for (int i = 0; i < v; i++) {
    vector<Edge> edges = gm->getNeighbors(i);
    for (int j = 0; j < edges.size(); j++) {
      if (gm->getIsDirected() || i <= edges[j].to) {
        gl.addEdge(i, edges[j].to, edges[j].weight);
      }
    }
  }
  return gl;
}

// Block 1
void dfs_conn(IGraph *g, int v, vector<bool> &vis) {
  vis[v] = true;
  vector<Edge> neighbors = g->getNeighbors(v);
  for (int i = 0; i < neighbors.size(); i++) {
    if (!vis[neighbors[i].to]) {
      dfs_conn(g, neighbors[i].to, vis);
    }
  }
}

bool isConnected(IGraph *g) {
  if (g->getNumVertices() == 0)
    return true;
  vector<bool> vis(g->getNumVertices(), false);
  dfs_conn(g, 0, vis);

  for (int i = 0; i < vis.size(); i++) {
    if (vis[i] == false)
      return false;
  }
  return true;
}

vector<vector<int>> getConnectedComponents(IGraph *g) {
  int v = g->getNumVertices();
  vector<bool> vis(v, false);
  vector<vector<int>> comps;

  for (int i = 0; i < v; i++) {
    if (!vis[i]) {
      vector<int> current_comp;
      stack<int> st;
      st.push(i);
      while (!st.empty()) {
        int curr = st.top();
        st.pop();
        if (!vis[curr]) {
          vis[curr] = true;
          current_comp.push_back(curr);
          vector<Edge> neighbors = g->getNeighbors(curr);
          for (int j = 0; j < neighbors.size(); j++) {
            if (!vis[neighbors[j].to]) {
              st.push(neighbors[j].to);
            }
          }
        }
      }
      comps.push_back(current_comp);
    }
  }
  return comps;
}

// Block 2: DFS and BFS
void DFS(IGraph *g, int start, bool sortWeight = false) {
  vector<bool> vis(g->getNumVertices(), false);
  stack<int> s;
  s.push(start);

  cout << "DFS: ";
  while (!s.empty()) {
    int v = s.top();
    s.pop();

    if (!vis[v]) {
      vis[v] = true;
      cout << v << " ";

      vector<Edge> neighbors = g->getNeighbors(v);
      if (sortWeight) {
        sort(neighbors.begin(), neighbors.end(), cmpEdgesDesc);
      }

      for (int i = 0; i < neighbors.size(); i++) {
        if (!vis[neighbors[i].to]) {
          s.push(neighbors[i].to);
        }
      }
    }
  }
  cout << endl;
}

void BFS(IGraph *g, int start, bool sortWeight = false) {
  vector<bool> vis(g->getNumVertices(), false);
  queue<int> q;
  q.push(start);
  vis[start] = true;

  cout << "BFS: ";
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    cout << v << " ";

    vector<Edge> neighbors = g->getNeighbors(v);
    if (sortWeight) {
      sort(neighbors.begin(), neighbors.end(), cmpEdges);
    }

    for (int i = 0; i < neighbors.size(); i++) {
      if (!vis[neighbors[i].to]) {
        vis[neighbors[i].to] = true;
        q.push(neighbors[i].to);
      }
    }
  }
  cout << endl;
}

// Block 3
vector<int> dijkstra(IGraph *g, int start) {
  int v_count = g->getNumVertices();
  vector<int> dist(v_count, INF);

  priority_queue<pair<int, int>, vector<pair<int, int>>,
                 greater<pair<int, int>>>
      pq;

  dist[start] = 0;
  pq.push(make_pair(0, start));

  while (!pq.empty()) {
    int d = pq.top().first;
    int u = pq.top().second;
    pq.pop();

    if (d > dist[u])
      continue;

    vector<Edge> neighbors = g->getNeighbors(u);
    for (int i = 0; i < neighbors.size(); i++) {
      int to = neighbors[i].to;
      int w = neighbors[i].weight;

      if (dist[u] + w < dist[to]) {
        dist[to] = dist[u] + w;
        pq.push(make_pair(dist[to], to));
      }
    }
  }
  return dist;
}

vector<vector<int>> floydWarshall(IGraph *g) {
  int v = g->getNumVertices();
  vector<vector<int>> dist(v, vector<int>(v, INF));

  for (int i = 0; i < v; i++) {
    dist[i][i] = 0;
    vector<Edge> neighbors = g->getNeighbors(i);
    for (int j = 0; j < neighbors.size(); j++) {
      dist[i][neighbors[j].to] = neighbors[j].weight;
    }
  }

  for (int k = 0; k < v; k++) {
    for (int i = 0; i < v; i++) {
      for (int j = 0; j < v; j++) {
        if (dist[i][k] != INF && dist[k][j] != INF &&
            dist[i][k] + dist[k][j] < dist[i][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
        }
      }
    }
  }
  return dist;
}

// Block 4: Topological sort
vector<int> kahn(IGraph *g) {
  int v = g->getNumVertices();
  vector<int> inDeg(v, 0);

  for (int i = 0; i < v; i++) {
    vector<Edge> edges = g->getNeighbors(i);
    for (int j = 0; j < edges.size(); j++) {
      inDeg[edges[j].to]++;
    }
  }

  queue<int> q;
  for (int i = 0; i < v; i++) {
    if (inDeg[i] == 0)
      q.push(i);
  }

  vector<int> order;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    order.push_back(u);

    vector<Edge> edges = g->getNeighbors(u);
    for (int j = 0; j < edges.size(); j++) {
      inDeg[edges[j].to]--;
      if (inDeg[edges[j].to] == 0) {
        q.push(edges[j].to);
      }
    }
  }

  if (order.size() != v) {
    cout << "Cycle detected!" << endl;
    return vector<int>();
  }
  return order;
}

void dfsTop(IGraph *g, int v, vector<bool> &vis, stack<int> &st) {
  vis[v] = true;
  vector<Edge> edges = g->getNeighbors(v);
  for (int i = 0; i < edges.size(); i++) {
    if (!vis[edges[i].to]) {
      dfsTop(g, edges[i].to, vis, st);
    }
  }
  st.push(v);
}

vector<int> dfsTopological(IGraph *g) {
  int v = g->getNumVertices();
  vector<bool> vis(v, false);
  stack<int> st;

  for (int i = 0; i < v; i++) {
    if (!vis[i]) {
      dfsTop(g, i, vis, st);
    }
  }

  vector<int> res;
  while (!st.empty()) {
    res.push_back(st.top());
    st.pop();
  }
  return res;
}

// Block 5: spanning tree bfs
GraphList bfsSpanningTree(IGraph *g, int start) {
  int v = g->getNumVertices();
  GraphList tree(v, g->getIsDirected());
  vector<bool> vis(v, false);
  queue<int> q;

  q.push(start);
  vis[start] = true;

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    vector<Edge> edges = g->getNeighbors(u);
    for (int i = 0; i < edges.size(); i++) {
      if (!vis[edges[i].to]) {
        vis[edges[i].to] = true;
        tree.addEdge(u, edges[i].to, edges[i].weight);
        q.push(edges[i].to);
      }
    }
  }
  return tree;
}

// Block 6: Prim and Kruskal
GraphList prim(IGraph *g, int &totalWeight) {
  int v = g->getNumVertices();
  GraphList mst(v, false);
  vector<bool> inMst(v, false);
  totalWeight = 0;

  priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>,
                 greater<pair<int, pair<int, int>>>>
      pq;

  inMst[0] = true;
  vector<Edge> edges = g->getNeighbors(0);
  for (int i = 0; i < edges.size(); i++) {
    pq.push(make_pair(edges[i].weight, make_pair(0, edges[i].to)));
  }

  while (!pq.empty()) {
    int w = pq.top().first;
    int u = pq.top().second.first;
    int to = pq.top().second.second;
    pq.pop();

    if (inMst[to])
      continue;

    inMst[to] = true;
    mst.addEdge(u, to, w);
    totalWeight += w;

    vector<Edge> nextEdges = g->getNeighbors(to);
    for (int i = 0; i < nextEdges.size(); i++) {
      if (!inMst[nextEdges[i].to]) {
        pq.push(make_pair(nextEdges[i].weight, make_pair(to, nextEdges[i].to)));
      }
    }
  }
  return mst;
}

struct DSU {
  int *parent;
  int *rank;
  DSU(int n) {
    parent = new int[n];
    rank = new int[n];
    for (int i = 0; i < n; i++) {
      parent[i] = i;
      rank[i] = 0;
    }
  }
  ~DSU() {
    delete[] parent;
    delete[] rank;
  }
  int find(int i) {
    if (parent[i] == i)
      return i;
    return parent[i] = find(parent[i]);
  }
  bool unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j) {
      if (rank[root_i] < rank[root_j]) {
        parent[root_i] = root_j;
      } else if (rank[root_i] > rank[root_j]) {
        parent[root_j] = root_i;
      } else {
        parent[root_j] = root_i;
        rank[root_i]++;
      }
      return true;
    }
    return false;
  }
};

struct EdgeData {
  int u, v, w;
};

bool cmpEdgeData(EdgeData a, EdgeData b) { return a.w < b.w; }

GraphList kruskal(IGraph *g, int &totalWeight) {
  int v = g->getNumVertices();
  GraphList mst(v, false);
  totalWeight = 0;

  vector<EdgeData> allEdges;
  for (int i = 0; i < v; i++) {
    vector<Edge> edges = g->getNeighbors(i);
    for (int j = 0; j < edges.size(); j++) {
      if (g->getIsDirected() || i < edges[j].to) {
        EdgeData ed;
        ed.u = i;
        ed.v = edges[j].to;
        ed.w = edges[j].weight;
        allEdges.push_back(ed);
      }
    }
  }

  sort(allEdges.begin(), allEdges.end(), cmpEdgeData);
  DSU dsu(v);

  for (int i = 0; i < allEdges.size(); i++) {
    int u = allEdges[i].u;
    int to = allEdges[i].v;
    int w = allEdges[i].w;

    if (dsu.unite(u, to)) {
      mst.addEdge(u, to, w);
      totalWeight += w;
    }
  }

  return mst;
}

void demo() {
  cout << endl << "--- Demo Mode ---" << endl;
  GraphMatrix gm(5, false);
  gm.addEdge(0, 1, 2);
  gm.addEdge(0, 3, 6);
  gm.addEdge(1, 2, 3);
  gm.addEdge(1, 3, 8);
  gm.addEdge(1, 4, 5);
  gm.addEdge(2, 4, 7);
  gm.addEdge(3, 4, 9);

  GraphList gl = convertMatrixToList(&gm);

  if (isConnected(&gl)) {
    cout << "Graph is connected" << endl;
  } else {
    cout << "Graph is not connected" << endl;
  }

  BFS(&gl, 0);
  DFS(&gl, 0);
  BFS(&gl, 0, true); // sorted by weight

  vector<int> dist = dijkstra(&gl, 0);
  cout << "Dijkstra from 0:" << endl;
  for (int i = 0; i < dist.size(); i++) {
    cout << "to " << i << " is " << dist[i] << endl;
  }

  int w1 = 0;
  prim(&gl, w1);
  cout << "Prim MST weight: " << w1 << endl;

  int w2 = 0;
  kruskal(&gl, w2);
  cout << "Kruskal MST weight: " << w2 << endl;

  gl.visualize();
}

void interactive() {
  cout << endl << "--- Interactive Mode ---" << endl;
  int v, e;
  bool dir;
  cout << "enter vertices count: ";
  cin >> v;
  cout << "is directed? (1/0): ";
  cin >> dir;

  GraphList gl(v, dir);

  cout << "enter edges count: ";
  cin >> e;
  cout << "enter edges (from to weight):" << endl;
  for (int i = 0; i < e; i++) {
    int from, to, w;
    cin >> from >> to >> w;
    gl.addEdge(from, to, w);
  }

  while (true) {
    cout << "\n1. BFS\n2. DFS\n3. Dijkstra\n4. Components\n5. Visualize\n6. "
            "Exit\n";
    cout << "choice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
      int start;
      cout << "start vertex: ";
      cin >> start;
      BFS(&gl, start);
    } else if (choice == 2) {
      int start;
      cout << "start vertex: ";
      cin >> start;
      DFS(&gl, start);
    } else if (choice == 3) {
      int start;
      cout << "start vertex: ";
      cin >> start;
      vector<int> d = dijkstra(&gl, start);
      for (int i = 0; i < d.size(); i++) {
        cout << "dist to " << i << " = " << d[i] << endl;
      }
    } else if (choice == 4) {
      vector<vector<int>> comps = getConnectedComponents(&gl);
      cout << "components count: " << comps.size() << endl;
      for (int i = 0; i < comps.size(); i++) {
        cout << "comp " << i + 1 << ": ";
        for (int j = 0; j < comps[i].size(); j++) {
          cout << comps[i][j] << " ";
        }
        cout << endl;
      }
    } else if (choice == 5) {
      gl.visualize();
    } else if (choice == 6) {
      break;
    } else {
      cout << "wrong choice" << endl;
    }
  }
}

void benchmark() {
  cout << endl << "--- Benchmark Mode ---" << endl;
  int v = 1000;
  int e = 5000;

  GraphMatrix gm(v, false);
  GraphList gl(v, false);

  mt19937 gen(123); // simple seed
  uniform_int_distribution<int> distV(0, v - 1);
  uniform_int_distribution<int> distW(1, 100);

  for (int i = 0; i < e; i++) {
    int from = distV(gen);
    int to = distV(gen);
    int w = distW(gen);
    gm.addEdge(from, to, w);
    gl.addEdge(from, to, w);
  }

  auto s1 = chrono::high_resolution_clock::now();
  dijkstra(&gm, 0);
  auto e1 = chrono::high_resolution_clock::now();

  auto s2 = chrono::high_resolution_clock::now();
  dijkstra(&gl, 0);
  auto e2 = chrono::high_resolution_clock::now();

  double t1 = chrono::duration<double, milli>(e1 - s1).count();
  double t2 = chrono::duration<double, milli>(e2 - s2).count();

  cout << "Matrix time: " << t1 << " ms" << endl;
  cout << "List time: " << t2 << " ms" << endl;
}

int main() {
  cout << "Lab Graph Algorithms" << endl;
  cout << "1. Demo" << endl;
  cout << "2. Interactive" << endl;
  cout << "3. Benchmark" << endl;
  cout << "choose: ";

  int c;
  cin >> c;
  if (c == 1) {
    demo();
  } else if (c == 2) {
    interactive();
  } else if (c == 3) {
    benchmark();
  } else {
    cout << "error" << endl;
  }

  return 0;
}
