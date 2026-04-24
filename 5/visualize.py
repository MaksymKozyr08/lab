import networkx as nx
import matplotlib.pyplot as plt

with open('graph_data.txt', 'r') as f:
    lines = f.readlines()
    v_count, is_directed = map(int, lines[0].split())
    if is_directed:
        G = nx.DiGraph()
    else:
        G = nx.Graph()
    G.add_nodes_from(range(v_count))
    for line in lines[1:]:
        u, v, w = map(int, line.split())
        G.add_edge(u, v, weight=w)

pos = nx.spring_layout(G)
nx.draw(G, pos, with_labels=True, node_color='skyblue', node_size=600)
labels = nx.get_edge_attributes(G, 'weight')
nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)
plt.show()
