#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

struct Edge {
	int from;
	int to;
	int capacity;
};

struct FlowNetwork {
	vector<Edge> edges;
	vector<vector<int>> adj;
	int num_nodes;

	FlowNetwork (int num_nodes) {
		this->num_nodes = num_nodes;
		adj.resize(num_nodes);
	}

	void add_edge(int from, int to, int capacity) {
		Edge e{from, to, capacity};
		edges.push_back(e);
		adj[from].push_back(edges.size() - 1);
		Edge e_rev{to, from, 0};
		edges.push_back(e_rev);
		adj[to].push_back(edges.size() - 1);
	}

	int get_edges_size() {
		return edges.size();
	}

	Edge get_edge(int index) {
		return edges[index];
	}

	vector<int> get_adj(int index) {
		return adj[index];
	}

	int get_num_nodes() {
		return num_nodes;
	}
};

int get_max_flow(FlowNetwork &network, int source, int sink) {
	int max_flow = 0;
	vector<Edge> residual_network_edges = network.edges;

	while (true) {
		vector<int> parents(network.get_num_nodes(), -1);
		vector<int> residual_network_edge_ids;
		queue<int> queue;

		queue.push(source);
		parents[source] = source;
		bool path_found = false;

		while (!queue.empty() && !path_found) {
			int current_node = queue.front();
			queue.pop();
			vector<int> current_node_adj = network.get_adj(current_node);

			for (int i = 0; i < current_node_adj.size(); i++) {
				Edge current_edge = residual_network_edges[current_node_adj[i]];
				int to = current_edge.to;
				
				if (parents[to] == -1 && current_edge.capacity > 0) {
					parents[to] = current_node_adj[i];
					residual_network_edge_ids.push_back(current_node_adj[i]);

					if (to == sink) {
						path_found = true;
						break;
					}

					queue.push(to);
				}
			}
		}

		if (!path_found) {
			break;
		}

		int path_flow = numeric_limits<int>::max();

		for (int i = 0; i < residual_network_edge_ids.size(); i++) {
			Edge current_edge = residual_network_edges[residual_network_edge_ids[i]];
			int from = current_edge.from;
			int to = current_edge.to;
			int capacity = current_edge.capacity;
			int current_path_flow = capacity;

			int current_node = to;
			int current_node_parent = parents[current_node];

			while (current_node != from) {
				Edge current_edge = residual_network_edges[current_node_parent];
				int current_edge_from = current_edge.from;
				int current_edge_capacity = current_edge.capacity;
				current_path_flow = min(current_path_flow, current_edge_capacity);
				current_node = current_edge_from;
				current_node_parent = parents[current_node];
			}

			path_flow = min(current_path_flow, path_flow);
		}

		int current_node = sink;
		int current_node_parent = parents[current_node];

		while (current_node != source) {
			Edge current_edge = residual_network_edges[current_node_parent];
			int current_edge_from = current_edge.from;
			int current_edge_to = current_edge.to;
			int current_edge_capacity = current_edge.capacity;
			residual_network_edges[current_node_parent].capacity -= path_flow;
			residual_network_edges[current_node_parent ^ 1].capacity += path_flow;
			current_node = current_edge_from;
			current_node_parent = parents[current_node];
		}

		max_flow += path_flow;
	}

	return max_flow;
}

int main() {
	int num_instances;
	cin >> num_instances;

	for (int i = 0; i < num_instances; i++) {
		int num_nodes;
		int num_edges;
		cin >> num_nodes >> num_edges;

		FlowNetwork network(num_nodes);

		for (int j = 0; j < num_edges; j++) {
			int from;
			int to;
			int capacity;
			cin >> from >> to >> capacity;
			network.add_edge(from - 1, to - 1, capacity);
		}

		int source = 0;
		int sink = num_nodes - 1;
		int max_flow = get_max_flow(network, source, sink);
		cout << max_flow << endl;
	}

	return 0;
}
