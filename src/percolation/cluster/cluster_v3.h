//
// Created by shahnoor on 7/1/19.
//

#ifndef NETWORK_CLUSTER_V3_H
#define NETWORK_CLUSTER_V3_H

#include <vector>
#include "../../nets/node.h"
#include "../../nets/link.h"

/**
 * Instead of using Link object it uses LinkIndex to make it memory efficient.
 */
class Cluster_v3{
    std::vector<uint> _nodes;

    int _group_id{-1};
    size_t _link_count{};

public:
    ~Cluster_v3() = default;
    Cluster_v3() = default;
    void reset() {
        _nodes.clear();
        _link_count = 0;
        _group_id=-1;

    }
    void set_group_id(int id) {_group_id = id;}
    int  get_group_id() const { return _group_id;}

    void add_node(const Node & nds);
    void add_link(const Link & lnk) {++_link_count;}
    void add_nodes(const std::vector<Node> & nds);
    void add_links(const std::vector<Link> & lnk){_link_count += lnk.size();}

    void insert(const Cluster_v3& clstr);

    const std::vector<uint>& getNodes() const { return _nodes;}

    // pass in the random number and get the desired node or link
    uint getNodeRandomly(size_t r) const {return _nodes[r % _nodes.size()];}

    size_t numberOfNodes() const { return _nodes.size();}
    size_t numberOfLinks() const { return _link_count;}

    void viewNodes();
//    void viewNodesExtended();

    void viewLinkIndices();
//    void viewLinkIndicesExtended();

    bool empty() const {return _nodes.empty() && _link_count==0;}

    void clear() {
        _nodes.clear();
        _link_count = 0;
    }

    double size_in_MB();
};

#endif //NETWORK_CLUSTER_V3_H
