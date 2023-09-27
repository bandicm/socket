#include <iostream>
#include <string>
#include <chrono>

#include "../lib/tcp_socket.hpp"

using namespace std;
using namespace chrono;

int main() {

    try {
        cout << "inicijalizacija reda" << endl;
        // queue sockets(2, "127.0.0.1", 5000, 500);


        // thread t1([&]() {
        //     cout << "korištenje prvog klijenta" << endl;
        //     int a = 0;
        //     while(true) {
        //         try {
        //             auto start = high_resolution_clock::now();
        //             client myserver = sockets.dequeue();
        //             auto stop = high_resolution_clock::now();
        //             auto duration = duration_cast<microseconds>(stop - start);
        //             cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
        //             string sends = "Socket I " + to_string(a++);
        //             myserver.push(sends);
        //             cout << myserver.pull() << endl; 

        //         } catch (const string err) {
        //             cout << err << endl;
        //         } 
        //         // usleep(100000);
        //     }
        // });

        // // usleep(20000);

        // thread t2([&]() {
        //     cout << "korištenje drugog klijenta" << endl;
        //     int a = 0;
        //     while(true) {
        //         try {
        //             auto start = high_resolution_clock::now();
        //             client myserver = sockets.dequeue();
        //             auto stop = high_resolution_clock::now();
        //             auto duration = duration_cast<microseconds>(stop - start);
        //             cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
        //             string sends = "Socket II " + to_string(a++);
        //             myserver.push(sends);
        //             cout << myserver.pull() << endl; 

        //         } catch (const string err) {
        //             cout << err << endl;
        //         } 
        //         // usleep(100000);
        //     }
        // });

        // t1.join();
        // t2.join();

        // int a = 0;
        // auto start = high_resolution_clock::now();
        // client myserver ("127.0.0.1", 5000, 500);
        // // auto stop = high_resolution_clock::now();
        // // auto duration = duration_cast<microseconds>(stop - start);
        // // cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
        // string sends = "Socket II " + to_string(a++);
        // myserver.push(sends);
        // cout << myserver.pull() << endl; 
        // auto stop = high_resolution_clock::now();
        // auto duration = duration_cast<microseconds>(stop - start);
        // cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
   

        vector<client*> clis;
        for (uint i =0 ; i<5; i++) {
            clis.push_back(new client("127.0.0.1", 5000, 500));
        }

        cout << "Dodalo je đubre "<< endl;
        sleep(5);
        string sends = "Socket I ";
        clis[1]->push(sends);
        cout << clis[1]->pull() << endl; 
        sends = "Socket III ";
        clis[3]->push(sends);
        cout << clis[3]->pull() << endl; 

    }
    catch (const string err) {
        cout << err << endl;
    }

    return 0;
}