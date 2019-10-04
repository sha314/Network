//
// Created by shahnoor on 12/19/18.
//

#ifndef NETWORK_TEST_NETWORK_PERCOLATION_H
#define NETWORK_TEST_NETWORK_PERCOLATION_H

#include <iostream>
#include <algorithm>



void network_percolation(int argc, char* argv[]);
void  explosive_percolation_sum(
        uint m, uint network_size, uint M, uint ensemble_size, size_t th_id);
void network_percolation_explosive(int argc, char* argv[]);

void network_percolation_global();

void network_percolationReverse_global();

void BA_self_jump(int argc, char* argv[]);
void BA_self_jump_avg(int argc, char* argv[]);


void test_NetworkBApercolationExplosive_v3(int argc, char **argv);
void test_NetworkBApercolationExplosive_v3_jump(int argc, char **argv);

void test_v5(int argc, char **argv);
void test_v3(int argc, char **argv);

void test_NetworkBA_v2(int argc, char **argv);


#endif //NETWORK_NETWORK_PERCOLATION_H
