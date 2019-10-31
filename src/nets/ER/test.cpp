//
// Created by shahnoor on 10/30/19.
//

#include "test.h"
#include "network_ER.h"
#include "../../percolation/network_percolation_v7.h"
using namespace std;


void test_ER() {
    Network_v2* net = new Network_ER(100, 0.8);
    net->setRandomState(0, true);
    net->initialize();
    net->view();
    net->viewLocal();

}


void test_ER_Percolation() {
    Network_v2* net = new Network_ER(10, 0.4);
    net->setRandomState(0, true);
    net->initialize();
//    net->view();
//    net->viewLocal();
    net->viewAdjacencyList();

//    NetworkPercolation_v7 percolation(net);
//    percolation.initialize();
//    percolation.viewClusters();
//    while(percolation.occupyLink()){
////        percolation.viewClusters();
////        cout << percolation.entropy_v1() << " , " << percolation.entropy_v2() << endl;
//        cout << percolation.largestClusterSize() << endl;
//    }
//    percolation.viewClusters();

}
