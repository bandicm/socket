#include <iostream>

#include "../lib/tcp_socket.hpp"

using namespace std;

int main() {

    try {

        secure crypto;
        cout << "init cert " << endl;

        client myserver("localhost", 8000, 5000, crypto.fds);
        // client myserver("localhost", 5000);
        cout << "init client " << endl;


        string sends = "Hello world!";
        cout << myserver.push(sends) << " " << sends.length() << endl;
        cout << "wait client " << endl;

        cout << myserver.pull();

    }
    catch (const string err) {
        cout << err << endl;
    }

    return 0;
}