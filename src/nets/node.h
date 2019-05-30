//
// Created by shahnoor on 8/9/18.
//

#ifndef NETWORK_NODE_H
#define NETWORK_NODE_H

#include <vector>
#include <ostream>

/**
 * Node of a network
 */
class Node{
    // it is also the index of the node in the list of the node in a network.
    // makes it easier to access them
    uint  _id;

    std::vector<uint> _connected_nodes;

    // group id is also the index of the cluster to which the node belongs.
    // very useful in percolation. makes it easier to find the cluster index
    // just from the group id of a node
    int _group_id{-1};

    // at i-th iteration m nodes are linked.
    // if nodes have same _current_group value that means the node is already selected
    uint _current_group{};
public:
    ~Node() = default;
//    ~Node() {_connected_nodes.clear();};
    Node(uint id):_id{id}{};
    Node(uint id, int groupId):_id{id}, _group_id{groupId}{};

    uint get_id() const { return  _id;}
    int get_group_id() const { return  _group_id;}
    void set_group_id(int id) {_group_id = id;}

    uint get_current_group() const {return _current_group;}
    void set_current_group(uint grp) {_current_group = grp;}

    const std::vector<uint>& get_neighbors() const {return _connected_nodes;}

    void add_neighbor(Node n);
    void addNeighbor(uint n);

    /**
     * Checked version
     * less efficient but useful for debugging
     */
    void add_neighbor_checked(Node n);
    void add_neighbor_checked(uint n);

    /**
     * Number of links a node have is the degree of that node
     */
    uint neighborCount() const {return _connected_nodes.size();}

    void shrink_to_fit() {_connected_nodes.shrink_to_fit();}
    double size_in_MB();

};


std::ostream& operator<<(std::ostream& os, const Node& node);

#endif //NETWORK_NODE_H
