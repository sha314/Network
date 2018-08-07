//
// Created by shahnoor on 8/7/18.
//

#include "network.h"

using  namespace std;


Link::Link(size_t id_a, size_t id_b) {
    if(id_a == id_b){
        cerr << "Self linking is not allowed : line " << __LINE__ << endl;
    } else{
        _id_node_a = id_a;
        _id_node_b = id_b;
    }

}


Node::Node(size_t id) {
    _id = id;
}

void Node::add_neighbor(Node n) {
    _connected_nodes.push_back(n.get_id());
}

void Node::add_neighbor(size_t n) {
    _connected_nodes.push_back(n);
}

void Node::add_neighbor_checked(Node n) {
    size_t i = n.get_id();
    for(size_t k{}; k < _connected_nodes.size(); ++k){
        if(i == _connected_nodes[k]){
            cerr << "Already a member : line " << __LINE__ << endl;
        }
    }
    _connected_nodes.push_back(i);

}

void Node::add_neighbor_checked(size_t n) {
    for(size_t k{}; k < _connected_nodes.size(); ++k){
        if(n == _connected_nodes[k]){
            cerr << "Already a member : line " << __LINE__ << endl;
        }
    }
    _connected_nodes.push_back(n);
}

std::ostream& operator<<(std::ostream& os, const Node& node){
    return os << node.get_id();
}

std::ostream& operator<<(std::ostream& os, const Link& lnk){
    return os << lnk.to_string();
}

/*****************************
 * Network Methods
 ****************************/

/**
 *
 * @param m0 : number of initial nodes
 * @param m  : number of links new nodes are born with
 */
Network::Network(size_t m0, size_t m) {
    if (m0 < m){
        _m0 = m;
        _m = m;
    }else{
        _m0 = m0;
        _m = m;
    }

    initialize();

}

void Network::initialize() {
    for(size_t i{}; i != _m0; ++i){
        _nodes.push_back({i}); // add node with id equal to the size of the _nodes
    }
    size_t k{};
    for(size_t i{}; i != _m0; ++i){
        k = (i+1) % _m0;
        _nodes[i].add_neighbor(k);
        _nodes[k].add_neighbor(i);
        _total_degree += 2;
        _links.push_back({k, i});
    }

//    for(size_t i{} ; i < _nodes.size(); ++i){
//        _total_degree += _nodes[i].degree();
//    }
}


/**
 * Add a new node to the network and connect with with_node nodes
 * @param with_node
 */
void Network::add_node(std::vector<size_t> with_node) {
    size_t sz = {_nodes.size()};
    _nodes.push_back({sz}); // add node with id equal to the size of the _nodes
    for(size_t n: with_node){
        _nodes[sz].add_neighbor(n);
        _nodes[n].add_neighbor(sz);
        _total_degree += 2; // one link  increases degree by 2.
        _links.push_back({sz, n});
    }
}

void Network::add_node(size_t with_node) {
    size_t sz = {_nodes.size()};
    _nodes.push_back({sz}); // add node with id equal to the size of the _nodes
    _nodes[sz].add_neighbor(with_node);
    _nodes[with_node].add_neighbor(sz);
    _total_degree += 2; // one link  increases degree by 2.
    _links.push_back({sz, with_node});
}
/**
 * Adds node randomly
 */
void Network::add_node() {
    size_t sz = _nodes.size();
    size_t r = rand() % sz;
    _nodes.push_back({sz}); // add node with id equal to the size of the _nodes
    _nodes[sz].add_neighbor(r);
    _nodes[r].add_neighbor(sz);
    _total_degree += 2; // one link  increases degree by 2.
    _links.push_back({sz, r});
}


/**
 *
 */
void Network::view_nodes() {
    cout << "id(size):{neighbors,...}" << endl;
    for(size_t i{}; i < _nodes.size(); ++i){
        cout << _nodes[i].get_id() << "(" << _nodes[i].degree() << "):{";
        auto neighbors = _nodes[i].get_neighbors();
        for(auto n: neighbors){
            cout << n << ',';
        }
        cout <<  "}" << endl;
    }
}

void Network::view_links() {
    cout << "links {" ;
    for(size_t i{}; i < _links.size(); ++i){
        cout << _links[i] << ',';
    }
    cout << "}" << endl;
}

vector<double> Network::degrees() {
    vector<double> degs(_nodes.size());

    for(size_t i{}; i < _nodes.size(); ++i){
        degs[i] = _nodes[i].degree();
    }
//    cout << degs.size() << " : line " << __LINE__ << endl;
//    cout << "line " << __LINE__ << endl;
    return degs;
}


/**************************************
 * NetworkBA methods
 **************************************/

/**
 * Add nodes preferentially
 */
void NetworkBA::add_node() {

    double p = rand() / double(RAND_MAX);

    double tmp{};
    size_t sz = _nodes.size();
//    cout << "probability " << p << endl;
    for(size_t i{}; i < sz; ++i){
        tmp +=  _nodes[i].degree() / get_total_degree();
        if(tmp >= p){
//            cout << "selected node " << i << " with " << tmp << endl;
            // add node
            _nodes.push_back({sz});
            _nodes[i].add_neighbor(sz);
            _nodes[sz].add_neighbor(i);
            _total_degree += 2; // one link  increases degree by 2.
            _links.push_back({sz, i});
            break;
        }
    }
}


/*************************
 * Percolation on BA network
 */
NetworkBApercolation::NetworkBApercolation(size_t m0, size_t m, size_t size)
        : NetworkBA(m0, m), _network_size{size}
{

}
