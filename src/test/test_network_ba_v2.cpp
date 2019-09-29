//
// Created by shahnoor on 7/1/19.
//

#include <iostream>
#include "test_network_ba_v2.h"
#include "../percolation/network_percolation_v5.h"

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


void test_ba_v2_2(int argc, char* argv[]) {

    int m = atoi(argv[1]);
    int N = atoi(argv[2]);

    NetworkBA_v2 ba(5, m);
    ba.setRandomState(0, false);
//    ba.view();
    ba.grow(N);
//    ba.view();

//    ba.view();

}

void test_ba_v2_3() {
    NetworkBApercolation_v5 net(3, 1, 100);
    net.setRandomState(0, false);
    net.init(false);
    while(net.occupyLink()){

    }
    net.viewClusters();

}
