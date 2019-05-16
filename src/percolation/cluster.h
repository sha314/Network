//
// Created by shahnoor on 8/9/18.
//

#ifndef NETWORK_CLUSTER_H
#define NETWORK_CLUSTER_H

#include <vector>
#include "../nets/node.h"
#include "../nets/link.h"

/**
 * Cluster of Links and Nodes
 */
class Cluster{
    std::vector<uint> _nodes;
    std::vector<Link> _links;
    int _group_id{-1};
public:
    ~Cluster() = default;
    Cluster() = default;
    void reset() {_nodes.clear(); _links.clear(); _group_id=-1;}
    void set_group_id(int id) {_group_id = id;}
    int  get_group_id() const { return _group_id;}

    void add_node(const Node & nds);
    void add_link(const Link & lnk);
    void add_nodes(const std::vector<Node> & nds);
    void add_links(const std::vector<Link> & lnk);

    void insert(const Cluster& clstr);

    const std::vector<uint>& getNodes() const { return _nodes;}
    const std::vector<Link>& getLinks() const { return _links;}

    // pass in the random number and get the desired node or link
    uint getNodeRandomly(size_t r) const {return _nodes[r % _nodes.size()];}
    Link getLinkRandomly(size_t r) const {return _links[r % _links.size()];}

    uint getNodeRandomlyAndRemove(size_t r) ;
    Link getLinkRandomlyAndRemove(size_t r) ;


    size_t numberOfNodes() const { return _nodes.size();}
    size_t numberOfLinks() const { return _links.size();}

    void viewNodes();
//    void viewNodesExtended();

    void viewLinks();
    void viewLinksExtended();
    bool empty() const {return _links.empty() && _nodes.empty();}
    void clear() {_links.clear(); _nodes.clear();}
};

#endif //NETWORK_CLUSTER_H

