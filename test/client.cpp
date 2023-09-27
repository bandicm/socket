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

        // uint i = 0;

        // auto t1 = high_resolution_clock::now();
        // client mycli("localhost", 7000);
        // auto t2 = high_resolution_clock::now();
        // cout << "Connected: " << duration_cast<microseconds>(t2 - t1).count() << endl;

        // while (true) {
        //     t2 = high_resolution_clock::now();
        //     mycli.push("Helllo " + to_string(i++));
        //     cout << "> " << mycli.pull() << endl;
        //     auto t3 = high_resolution_clock::now();
        //     cout << "Sending and recive: " << duration_cast<microseconds>(t3 - t2).count() << endl;
        //     // usleep(10000);
        // }

        clientPool clies(5, "localhost", 7000);

        thread t1([&]() {
            int i = 0;
            while (true) {
                auto t1 = high_resolution_clock::now();
                auto socks = clies.pickup();
                auto t2 = high_resolution_clock::now();
                cout << "Picking : " << duration_cast<microseconds>(t2 - t1).count() << endl;
                socks->second->push("Helllo I " + to_string(i++));
                cout << "> " << socks->second->pull() << endl;
                auto t3 = high_resolution_clock::now();
                cout << "Sending and recive: " << duration_cast<microseconds>(t3 - t2).count() << endl;
                clies.release(socks);
            }
        });


        thread t2([&]() {
            int i = 0;
            while (true) {
                auto t1 = high_resolution_clock::now();
                auto socks = clies.pickup();
                auto t2 = high_resolution_clock::now();
                cout << "Picking : " << duration_cast<microseconds>(t2 - t1).count() << endl;
                socks->second->push("Helllo II " + to_string(i++));
                cout << "> " << socks->second->pull() << endl;
                auto t3 = high_resolution_clock::now();
                cout << "Sending and recive: " << duration_cast<microseconds>(t3 - t2).count() << endl;
                clies.release(socks);
            }
        });


        thread t3([&]() {
            int i = 0;
            while (true) {
                auto t1 = high_resolution_clock::now();
                auto socks = clies.pickup();
                auto t2 = high_resolution_clock::now();
                cout << "Picking : " << duration_cast<microseconds>(t2 - t1).count() << endl;
                socks->second->push("Helllo III " + to_string(i++));
                cout << "> " << socks->second->pull() << endl;
                auto t3 = high_resolution_clock::now();
                cout << "Sending and recive: " << duration_cast<microseconds>(t3 - t2).count() << endl;
                clies.release(socks);
            }
        });


        t1.join();
        t2.join();
        t3.join();

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

    }
    catch (const string err) {
        cout << err << endl;
    }

    return 0;
}