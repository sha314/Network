#include <iostream>
#include <chrono>
#include "src/util/time_tracking.h"
#include "src/network.h"

using namespace std;


void run_in_main(int argc, char* argv[]){
    NetworkBA net(3);
    net.view_nodes();
    net.view_links();

    cout << net.get_total_degree() << endl;
    size_t network_size = 5;
    for(size_t i{}; i < network_size ; ++i){
        net.add_node();
        net.view_nodes();
        net.view_links();
    }

    net.view_nodes();
    net.view_links();
}


/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    std::cout << "Netork" << std::endl;
    cout << currentTime() << endl;
    cout << "Compiled on " << __DATE__ << "\t at " << __TIME__ << endl;
    auto t_start = std::chrono::system_clock::now();

    time_t seed = time(nullptr);
    srand(seed);    // seeding

    run_in_main(argc, argv);

    auto t_end= std::chrono::system_clock::now();
    std::chrono::duration<double> drtion = t_end - t_start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(t_end);
    cout << "Program finished at " << std::ctime(&end_time) << endl;
    std::cout << "Time elapsed "   << getFormattedTime(drtion.count()) << std::endl;
    return 0;

}