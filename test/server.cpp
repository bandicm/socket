#include <iostream>

#include "../lib/tcp_socket.hpp"

using namespace std;

int main() {

    server myserver(5000, 10);
    comming myclient(&myserver, 1000);

    string fromclient =  myclient.obey();
    myclient.tell(fromclient);
    myclient.~comming();

    while (true) {
        comming myclient(&myserver, 1000);
        string fromclient =  myclient.obey();
        cout << fromclient << " " << myclient.conn << endl;
        cout << "Poslano: " << myclient.tell(fromclient) << "Veličin: " << fromclient.length() << endl;
        // myclient.~comming();
        cout << "IPv4 " << myclient.ipv4 << " ipv6 " << myclient.ipv6;
    }


    return 0;
}