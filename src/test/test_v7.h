//
// Created by shahnoor on 11/2/19.
//

#ifndef NETWORK_TEST_V7_H
#define NETWORK_TEST_V7_H

void test_v7(int argc, char* argv[]);
void test_ER();
void test_ER_Percolation();
void test_ER_PercolationExplosive();
void run_ER_percolation(int argc, char **argv);
void test_BA(int argc, char* argv[]);
void test_MDA(int argc, char* argv[]);
void test_percolation(int argc, char **argv);

void run_v7_percolation(int argc, char **argv);
void run_v7_percolation_jump(int argc, char **argv);
void run_v7_percolation_jump_avg(int argc, char **argv);

void run_v7_percolation_old_susceptibility(int argc, char **argv);// 2020.04.02
void run_v7_percolation_1st_order_check(int argc, char **argv);// 2020.05.13
void run_v7_percolation_1st_order_check_2(int argc, char **argv);// 2020.05.19


void run_v7_percolation_near_tc(int argc, char **argv); // 2019.12.06

void run_v7_percolation_delta_powder_keg(int argc, char **argv); // 2021.08.11

#endif //NETWORK_TEST_V7_H
