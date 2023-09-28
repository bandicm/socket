#include <iostream>
#include <string>
#include <chrono>

#include "../lib/tcp_socket.hpp"

using namespace std;
using namespace chrono;

int main() {

    try {

        // uint n = 10000;

        // vector<thread> thr;
        // for (uint i=0; i<n; i++) {
        //     thr.push_back(thread([](uint a){
        //         client myserver("127.0.0.1", 5000, 500);
        //         string sends = "Hello world " + to_string(a);
        //         myserver.push(sends);
        //         cout << myserver.pull() << endl;                
        //     }, i));
        // }

        // for (uint i=0; i<n; i++) {
        //     thr[i].join();
        // } 

        // secure crypto;
        // cout << "init cert " << endl;

        // client myserver("127.0.0.1", 5000, 5000, crypto.fds);
        // client myserver("localhost", 8000, 5000, crypto.fds);
        // client myserver("localhost", 5000);
        // cout << "init client " << endl;


        // string sends = "Hello world!";
        // cout << myserver.push(sends) << " " << sends.length() << endl;
        // cout << "wait client " << endl;

        // cout << myserver.pull();

auto t1 = high_resolution_clock::now();
            client mycli("127.0.0.1", 5000);
            auto t2 = high_resolution_clock::now();
            cout << "Connecting : " << duration_cast<microseconds>(t2 - t1).count() << endl;



        while (true) {


            
            
            auto t3 = high_resolution_clock::now();
            mycli.push("Helooo");
            auto t4 = high_resolution_clock::now();
            cout << "Send : " << duration_cast<microseconds>(t4 - t3).count() << endl;
            auto t5 = high_resolution_clock::now();
            string msg;
            try {
                msg = mycli.pull();
            } catch (const ConnectionException err) {
                cout << err.what() << endl;
                msg = err.getData();
            }
            cout << msg << endl;
            auto t6 = high_resolution_clock::now();
            cout << "Recive : " << duration_cast<microseconds>(t6 - t5).count() << endl;
            // break;
        }

    }
    
    catch (const string err) {
        cout << err << endl;
    }

    return 0;
}