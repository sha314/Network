//
// Created by shahnoor on 8/9/18.
//

#include <ostream>
#include <iostream>
#include "link.h"

using namespace std;

Link::Link(uint id_a, uint id_b) {
    if(id_a == id_b){
        cout << "between " << id_a << " and " << id_b << endl;
        cerr << "Self linking is not allowed : line " << __LINE__ << endl;
    } else{
        _id_node_a = id_a;
        _id_node_b = id_b;
    }

}


std::ostream& operator<<(std::ostream& os, const Link& lnk){
    return os << lnk.to_string();
}
