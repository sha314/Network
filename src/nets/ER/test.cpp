//
// Created by shahnoor on 10/30/19.
//

#include "test.h"
#include "network_ER.h"

void test_ER() {
    Network_v2* net = new Network_ER(10, 0.4);
    net->initialize();
    net->view();

}
