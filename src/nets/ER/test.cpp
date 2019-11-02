//
// Created by shahnoor on 10/30/19.
//

#include <chrono>
#include <fstream>
#include "test.h"
#include "network_ER.h"
#include "../../percolation/network_percolation_v7.h"
#include "../../util/time_tracking.h"

using namespace std;

//
//void test_ER() {
//    Network_v7* net = new NetworkER_v7(0.8);
//    net->setRandomState(0, true);
//    net->initialize(100);
//    net->view();
//    net->viewLocal();
//
//}
//
//
//void test_ER_Percolation() {
//    Network_v7* net=new NetworkER_v7(0.4);
//    net->setRandomState(0, true);
//    net->initialize(10);
////    net->view();
////    net->viewLocal();
//    net->viewAdjacencyList();
//
//    NetworkPercolation_v7 percolation(net);
//    percolation.initialize();
//    percolation.viewClusters();
//    while(percolation.occupyLink()){
////        percolation.viewClusters();
////        cout << percolation.entropy_v1() << " , " << percolation.entropy_v2() << endl;
//        cout << percolation.largestClusterSize() << endl;
//    }
//    percolation.viewClusters();
//
//}
//
//void test_ER_PercolationExplosive() {
//    auto * net = new NetworkER_v7(0.4);
//    net->setRandomState(0, false);
//    net->initialize(10);
////    net->view();
////    net->viewLocal();
//    net->viewAdjacencyList();
//
//    NetworkPercolationExplosive_v7 percolation(net, 2);
////    NetworkPercolationInverted_v7 percolation(net, 2);
//
//    percolation.initialize();
//    percolation.viewClusters();
//    while(percolation.occupyLink()){
////        percolation.viewClusters();
////        cout << percolation.entropy_v1() << " , " << percolation.entropy_v2() << endl;
//        cout << percolation.largestClusterSize() << endl;
//    }
//    percolation.viewClusters();
//
//}
//
//void run_ER_percolation(int argc, char* argv[]){
//    if(argc < 5 ){
//        cout << "argv[1] == N" << endl;
//        cout << "argv[2] == p" << endl;
//        cout << "argv[3] == M" << endl;
//        cout << "argv[4] == Ensemble" << endl;
//        return;
//    }
//    int N = atoi(argv[1]);
//    double p = atof(argv[2]);
//    int M = atoi(argv[3]);
//    int ensemble_size = atoi(argv[4]);
//    cout << "N="<< N << ",p=" << p << ",M=" << M << ",En="<<ensemble_size << endl;
//
//    auto * net=new NetworkER_v7(p);
//    net->setRandomState(0, true);
//    net->initialize(N);
//
////    NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
////    NetworkPercolationExplosive_v7 percolation(net, M);
//
//    percolation.initialize();
//    size_t linkCount = net->getLinkCount();
//    size_t nodeCount = net->getNodeCount();
//    cout << nodeCount << ", " << linkCount << ", " << endl;
////    net.viewNetwork();
////    net.viewListOfLinkIndices();
//    double entropy_jump{}, order_jump{};
//    vector<double> entropy(linkCount), order_param(linkCount); // entropy and order parameter
//    for (size_t k{0}; k < ensemble_size; ++k) {
//        auto t_start= chrono::_V2::system_clock::now();
////        net.viewListOfLinkIndices();
//        percolation.reset(0); // every 25 step. reset the network
////        cout << net->getLinkCount() << endl;
//        size_t i{};
////        net.viewClusters();
////        net.viewListOfLinkIndices();
////        net.viewNetwork();
////        cout << "entering to while" << endl;
//        while (percolation.occupyLink()) {
////            cout << "i " << i  << endl;
//            entropy[i] += percolation.entropy();
//            order_param[i] += percolation.largestClusterSize();
////            net.viewClusters();
////            cout << net.entropy_v1()  << "\t";
////            cout << net.entropy_v2() << endl;
////            cout << net.largestClusterSize() << endl;
//            percolation.jump();
////            _network_frame.viewClusterExtended();
//            ++i;
//            if (i >= linkCount) {
//                cout << "breaking at " << i <<endl;
//                break;
//            }
//        }
//        entropy_jump += percolation.largestEntropyJump();
//        order_jump += percolation.largestOrderJump();
//
////        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
//        auto t_end= chrono::_V2::system_clock::now();
//        chrono::duration<double> drtion = t_end - t_start;
//        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
//    }
//
//    auto tm = currentTime();
////    string signature = percolation.get_signature();
//    string signature = net->get_signature_array()[0] +"_"+ percolation.getClassName()  + "_N_" + to_string(N) + "_p_";
//    std::ostringstream out;
//    out.precision(3);
//    out << std::fixed << p;
//    signature +=  out.str() + "_M_" + to_string(M);
//    stringstream ss;
//    ss << "{"
//       << R"*("signature":")*" << signature << "\""
//       << R"*(,"network_class":")*" << net->getClassName() << "\""
//       << R"*(,"percolation_class":")*" << percolation.getClassName() << "\""
//       << R"*(,"p":)*" << p
//       << R"*(,"N":)*" << N
//       << R"*(,"number_of_links":)*" << linkCount
//       << R"*(,"number_of_nodes":)*" << nodeCount
//       << R"*(,"M":)*" << M
//       << R"*(,"ensemble_size":)*" << ensemble_size
//       << R"*(,"date":")*" << tm << "\""
//       << "}";
//
//
//
//    string filename = signature + "_entropy-order_" + tm + ".txt";
//    ofstream fout(filename);
//    fout << '#' << ss.str() << endl;
//    fout << "# t=relative link density" << endl;
//    fout << "#<t>\t<H>\t<P>" << endl;
//    for(size_t k{}; k < entropy.size() ; ++k){
//        auto t = (k+1)/double(N);
//        if (t > 5) {
//            cout << "breaking at t=" << t << endl;
//            break;
//        }
//        fout << t
//             << "\t" << entropy[k]/ensemble_size
//             << "\t" << order_param[k]/(ensemble_size*double(N)) << endl;
//    }
//    fout.close();
//}
