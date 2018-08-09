//
// Created by shahnoor on 8/9/18.
//

#ifndef NETWORK_LINK_H
#define NETWORK_LINK_H


#include <sstream>

/**
 *
 */
class Link{
    //size_t _id;
    int _group_id{-1}; // will be used for percolation
    bool _active{false};
    size_t _id_node_a, _id_node_b;
public:
    ~Link() = default;
    Link() = default;
    Link(size_t id_a, size_t id_b);

    int get_group_id() const { return  _group_id;}
    void set_group_id(int id) {_group_id = id;}

    bool is_active() const { return _active;}
    void activate() {_active = true;}

    std::string to_string() const {
        std::stringstream ss;
        ss << "(" << _id_node_a  << "," << _id_node_b << ")";
        return ss.str();
    }

    size_t get_a() const {return _id_node_a;}
    size_t get_b() const {return _id_node_b;}
};

std::ostream& operator<<(std::ostream& os, const Link& lnk);

#endif //NETWORK_LINK_H
