#!/usr/bin/python

# dijkstra.py
# Russell Miller
# Copyright 2011
# Portland State University CS 441
# Algorithm taken from http://en.wikipedia.org/wiki/Dijkstra's_algorithm


# Assign to every node a distance value: set it to zero for our initial node and to infinity for all other nodes.

# Mark all nodes as unvisited. Set initial node as current.

# For current node, consider all its unvisited neighbors and calculate their tentative distance (from the initial node). For example, if current node (A) has distance of 6, and an edge connecting it with another node (B) is 2, the distance to B through A will be 6+2=8. If this distance is less than the previously recorded distance (infinity in the beginning, zero for the initial node), overwrite the distance.

# When we are done considering all neighbors of the current node, mark it as visited. A visited node will not be checked ever again; its distance recorded now is final and minimal.

# If all nodes have been visited, finish. Otherwise, set the unvisited node with the smallest distance (from the initial node, considering all nodes in graph) as the next "current node" and continue from step 3.

#######################################################
# Nodes - label, neighbors, tentative_distance, visited

class Node:
    """A node in a state search graph"""
    def __init__(self, label, neighbors, dist):
        self.label=label
        self.neighbors=neighbors
        self.dist=dist
        self.visited=False


def dijkstra():
    """Dijkstra's Algorithm"""
    node_c = Node('C', None, 0)
    node_b = Node('B', [(node_c, 6)], 0)
    node_a = Node('A', [(node_b, 3)], 0)
    print('3 New Nodes. All visited?')
    print(allvisited([node_a, node_b, node_c]))
    print('Visiting A... All visited?')
    node_a.visited=True
    print(allvisited([node_a, node_b, node_c]))
    print('Visiting B... All visited?')
    node_b.visited=True
    print(allvisited([node_a, node_b, node_c]))
    print('Visiting C... All visited?')
    node_c.visited=True
    print(allvisited([node_a, node_b, node_c]))
    


def allvisited(nodes):
    """Helper for Dijkstra - Check if all nodes have been visited"""
    for node in nodes:
        if not node.visited: 
            return False
    return True


if __name__=="__main__":
    dijkstra()
