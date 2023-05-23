#include <iostream>

#include "../lib/tcp_socket.hpp"

using namespace std;

int main() {

    try {

    secure crypto;
    client myserver("localhost", 5000, 500, crypto.fds);
    string sends = "Hello world!";
    cout << myserver.tell(sends) << " " << sends.length() << endl;
    cout << myserver.obey();

    }
    catch (const string err) {
        cout << err << endl;
    }

    return 0;
}