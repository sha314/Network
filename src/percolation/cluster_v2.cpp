//
// Created by shahnoor on 5/27/19.
//


//
// Created by shahnoor on 8/9/18.
//

#include <iostream>
#include "cluster.h"

using namespace std;

void Cluster_v2::add_nodes(const std::vector<Node> &nds) {
    unsigned long sz = _nodes.size();
    _nodes.resize(sz + nds.size());
    for(size_t i{}; i < nds.size(); ++i){
        _nodes[sz + i] = nds[i].get_id();
    }
}


void Cluster_v2::add_link_indices(const std::vector<Link> &nds) {
    unsigned long sz = _links_indices.size();
    _links_indices.resize(sz + nds.size());
    for(size_t i{}; i < nds.size(); ++i){
        _links_indices[sz + i] = nds[i].getIndex();
    }
}


void Cluster_v2::add_link_index(const Link &lnk) {
    _links_indices.push_back(lnk.getIndex());
}

void Cluster_v2::add_node(const Node &nds) {
    _nodes.push_back(nds.get_id());
}

void Cluster_v2::viewNodes() {
    cout << "{";
    for(size_t i{}; i < _nodes.size(); ++i){
        cout << _nodes[i] << ',';
    }
    cout << "}";
}


void Cluster_v2::viewLinkIndices() {
    cout << "{";
    for(size_t i{}; i < _links_indices.size(); ++i){
        cout << _links_indices[i] << ',';
    }
    cout << "}";
}


void Cluster_v2::insert(const Cluster_v2 &clstr) {
    _nodes.insert(_nodes.end(), clstr._nodes.begin(), clstr._nodes.end());
    _links_indices.insert(_links_indices.end(), clstr._links_indices.begin(), clstr._links_indices.end());
}

double Cluster_v2::size_in_MB() {

    double sz = sizeof(int) + sizeof(uint) * _nodes.size() + sizeof(Link) * _links_indices.size();
    sz += sizeof(std::vector<uint>);
    sz += sizeof(std::vector<LinkIndex>);
//    cout << "sizeof  Cluster" << sz/(1024*1024) << " MB" << endl;
    return sz/(1024*1024);
}





