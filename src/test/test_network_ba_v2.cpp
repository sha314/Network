//
// Created by shahnoor on 7/1/19.
//

#include <iostream>
#include "test_network_ba_v2.h"

using namespace std;


void test_ba_v2() {
    NetworkBA_v2 ba(5, 3);
    ba.setRandomState(0, false);
    ba.setMaxNetworkSize(10);
    ba.view();
    size_t i = 0;
    cout << ba.getBlankSize() << endl;
    while(ba.spaceAvailable()) {
        ba.addNode();
        ba.view();
        ++i;
    }
//    ba.view();

}


void test_ba_v2_2() {
    NetworkBA_v2 ba(5, 3);
    ba.setRandomState(0, false);
    ba.view();
    ba.grow(100);
    ba.view();

//    ba.view();

}
