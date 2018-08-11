//
// Created by shahnoor on 8/9/18.
//

#include <iostream>
#include "cluster.h"

using namespace std;

void Cluster::add_nodes(const std::vector<Node> &nds) {
    unsigned long sz = _nodes.size();
    _nodes.resize(sz + nds.size());
    for(size_t i{}; i < nds.size(); ++i){
        _nodes[sz + i] = nds[i].get_id();
    }
}

void Cluster::add_links(const std::vector<Link> &nds) {
    unsigned long sz = _links.size();
    _links.resize(sz + nds.size());
    for(size_t i{}; i < nds.size(); ++i){
        _links[sz + i] = nds[i];
    }
}

void Cluster::add_link(const Link &lnk) {
    _links.push_back(lnk);
}

void Cluster::add_node(const Node &nds) {
    _nodes.push_back(nds.get_id());
}

void Cluster::viewNodes() {
    cout << "{";
    for(size_t i{}; i < _nodes.size(); ++i){
        cout << _nodes[i] << ',';
    }
    cout << "}";
}


void Cluster::viewLinks() {
    cout << "{";
    for(size_t i{}; i < _links.size(); ++i){
        cout << _links[i] << ',';
    }
    cout << "}";
}

void Cluster::viewLinksExtended() {
    cout << "{";
    for(size_t i{}; i < _links.size(); ++i){
        cout << _links[i] .get_group_id() << _links[i] << ',';
    }
    cout << "}";
}

void Cluster::insert(const Cluster &clstr) {
    _nodes.insert(_nodes.end(), clstr._nodes.begin(), clstr._nodes.end());
    _links.insert(_links.end(), clstr._links.begin(), clstr._links.end());
}


