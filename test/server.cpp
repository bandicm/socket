#include <iostream>

#include "../lib/tcp_socket.hpp"

using namespace std;

int main() {

    cout << "init server " << endl;
    server myserver(5000, 10);
    cout << "init cert " << endl;
    secure crypto ("../example/cert.pem", "../example/privkey.pem");
    cout << "init client " << endl;

    comming myclient(&myserver, 1000, crypto.fds);
    cout << "wait client " << myclient.ipv4 << endl;

    string fromclient =  myclient.obey();
    cout << "tell client " << fromclient << endl;
    sleep(5);
    myclient.tell(fromclient);
    // myclient.~comming();

    // while (true) {
    //     comming myclient(&myserver, 1000);
    //     string fromclient =  myclient.obey();
    //     cout << fromclient << " " << myclient.conn << endl;
    //     cout << "Poslano: " << myclient.tell(fromclient) << "Veličin: " << fromclient.length() << endl;
    //     // myclient.~comming();
    //     cout << "IPv4 " << myclient.ipv4 << " ipv6 " << myclient.ipv6;
    // }

    // sleep(80);


    return 0;
}