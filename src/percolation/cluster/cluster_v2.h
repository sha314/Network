//
// Created by shahnoor on 7/1/19.
//

#ifndef NETWORK_CLUSTER_V2_H
#define NETWORK_CLUSTER_V2_H

#include <vector>
#include "../../nets/node.h"
#include "../../nets/link.h"

/**
 * Instead of using Link object it uses LinkIndex to make it memory efficient.
 */
class Cluster_v2{
    std::vector<uint> _nodes;
    std::vector<LinkIndex> _links_indices; // instead of using Link. LinkIndex is memory efficient
    int _group_id{-1};

public:
    ~Cluster_v2() = default;
    Cluster_v2() = default;
    void reset() {
        _nodes.clear();
//        _links.clear();
        _links_indices.clear();
        _group_id=-1;

    }
    void set_group_id(int id) {_group_id = id;}
    int  get_group_id() const { return _group_id;}

    void add_node(const Node & nds);
    void add_link_index(const Link & lnk);
    void add_nodes(const std::vector<Node> & nds);
    void add_link_indices(const std::vector<Link> & lnk);

    void insert(const Cluster_v2& clstr);

    const std::vector<uint>& getNodes() const { return _nodes;}
//    const std::vector<Link>& getLinks() const { return _links;}
    const std::vector<LinkIndex>& getLinkIndices() const { return _links_indices;}

    // pass in the random number and get the desired node or link
    uint getNodeRandomly(size_t r) const {return _nodes[r % _nodes.size()];}
    LinkIndex getLinkRandomly(size_t r) const {return _links_indices[r % _links_indices.size()];}


    size_t numberOfNodes() const { return _nodes.size();}
//    size_t numberOfLinks() const { return _links.size();}
    size_t numberOfLinks() const { return _links_indices.size();}

    void viewNodes();
//    void viewNodesExtended();

    void viewLinkIndices();
//    void viewLinkIndicesExtended();

    bool empty() const {return _links_indices.empty() && _nodes.empty();}
    void clear() {
        _nodes.clear();
        _links_indices.clear();
    }

    double size_in_MB();
};



#endif //NETWORK_CLUSTER_V2_H
