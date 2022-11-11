# network_flow_Ford-Fulkerson
```
The code is used to find the shortest path from source node to sink node. 
The code uses a queue which is a data structure that can be implemented using an array or linked list.
The queue will hold the nodes in order of their distance from source node and will keep popping nodes off until it reaches sink node.
It then checks if there is a path found by checking if the queue is empty, and if not, it breaks out of loop and returns false as result.
It then iterates through all edges in residual network graph and calculates the flow between two adjacent nodes (current_node-from) by calculating capacity first, then finding minimum value of current_path_flow - current_edge_capacity.
```
