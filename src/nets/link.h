//
// Created by shahnoor on 8/9/18.
//

#ifndef NETWORK_LINK_H
#define NETWORK_LINK_H


#include <sstream>

/**
 * Index of a link consists of two node ids
 */
class LinkIndex{
    uint node_a;
    uint node_b;
public:
    LinkIndex(uint a, uint b): node_a{a}, node_b{b} {
        if(node_a == node_b) {
            std::cout << "between " << node_a << " and " << node_b << std::endl;
            std::cerr << "Self linking is not allowed : line " << __LINE__ << std::endl;
        }
    }
    std::string to_string() const {
        std::stringstream ss;
        ss << "(" << node_a  << "," << node_b << ")";
        return ss.str();
    }

    uint get_a() const {return node_a;}
    uint get_b() const {return node_b;}
};


/**
 * A like connects two nodes
 */
class Link{
    //uint _id;
    int _group_id{-1}; // will be used for percolation
    bool _active{false};
    uint _id_node_a, _id_node_b;
public:
    ~Link() = default;
    Link() = default;
    Link(uint id_a, uint id_b);

    int get_group_id() const { return  _group_id;}
    void set_group_id(int id) {_group_id = id;}

    bool isActive() const { return _active;}
    void activate() {_active = true;}

    std::string to_string() const {
        std::stringstream ss;
        ss << "(" << _id_node_a  << "," << _id_node_b << ")";
        return ss.str();
    }

    uint get_a() const {return _id_node_a;}
    uint get_b() const {return _id_node_b;}
};

std::ostream& operator<<(std::ostream& os, const Link& lnk);
std::ostream& operator<<(std::ostream& os, const LinkIndex& lnk);

#endif //NETWORK_LINK_H
