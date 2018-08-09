//
// Created by shahnoor on 8/9/18.
//

#ifndef NETWORK_CLUSTER_H
#define NETWORK_CLUSTER_H

#include <vector>
#include "nets/node.h"
#include "nets/link.h"

/**
 * Cluster of Links and Nodes
 */
class Cluster{
    std::vector<Node> _nodes;
    std::vector<Link> _links;
    int _group_id{-1};
public:
    ~Cluster() = default;
    Cluster() = default;

    void set_group_id(int id) {_group_id = id;}
    int  set_group_id() const { return _group_id;}

    void add_nodes(const std::vector<Node> & nds);
    void add_links(const std::vector<Link> & nds);
};

#endif //NETWORK_CLUSTER_H
