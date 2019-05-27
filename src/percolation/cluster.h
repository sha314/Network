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
};


/**
 * Instead of using Link object it uses LinkIndex to make it memory efficient.
 */
class Cluster_v3{
    std::vector<uint> _nodes;

    int _group_id{-1};
    size_t _link_count{};
    size_t _node_count{};
public:
    ~Cluster_v3() = default;
    Cluster_v3() = default;
    void reset() {
        _nodes.clear();
        _link_count = 0;
        _node_count = 0;
        _group_id=-1;

    }
    void set_group_id(int id) {_group_id = id;}
    int  get_group_id() const { return _group_id;}

    void add_node(const Node & nds);
    void add_link(const Link & lnk) {++_link_count;}
    void add_nodes(const std::vector<Node> & nds);
    void add_links(const std::vector<Link> & lnk){_link_count += lnk.size();}

    void insert(const Cluster_v2& clstr);

    const std::vector<uint>& getNodes() const { return _nodes;}

    // pass in the random number and get the desired node or link
    uint getNodeRandomly(size_t r) const {return _nodes[r % _nodes.size()];}

    size_t numberOfNodes() const { return _nodes.size();}

    void viewNodes();
//    void viewNodesExtended();

    void viewLinkIndices();
//    void viewLinkIndicesExtended();

    bool empty() const {return _nodes.empty() && _link_count==0;}

    void clear() {
        _nodes.clear();
        _link_count = 0;
        _node_count = 0;
    }
};

#endif //NETWORK_CLUSTER_H

