#include "../lib/tcp_socket.hpp"

/**
 * Kontrustruktor varijable tipa server, prima port i limit za ograničenje liste klijenata na čekanju
*/

server::server (const ushort port, const uint queue, SSL_CTX* _securefds) {
    securefds = _securefds;         

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    #if _WIN32
        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
            throw string("[ERROR] WSA Startup. Detail: " + to_string(WSAGetLastError()));
        }
    #endif

    sock = socket(AF_INET, SOCK_STREAM, 0); 
    if (sock <= 0) {  
        throw string("[ERROR] Unable to open TCP socket ");

    }

    int opt=1;
    #if __linux__
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            throw string("[ERROR] Unable to set REUSEADDR or REUSEPORT on socket ");
        }
    #elif _WIN32
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt))) {
            throw string("[ERROR] Unable to set REUSEADDR or REUSEPORT on socket ");
        }
    #endif
    

    if (bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
        throw string("[ERROR] Unable to bind socket ");
    }

    if (listen(sock, queue) < 0) {
        throw string("[ERROR] It is not possible to set the allowed number of waiting clients ");
    }

}

/**
 * Metoda za sinkroni rad s klijentima, prima pokazivač na funkciju i timeout;
 * Funkcija handlecli prima referencu tipa client - važno za definiranje funkcija koje se šalju;
 * Nije moguće proslijediti druge parametre;
*/

void server::sync(void (*handlecli)(client&), const uint timeout) {
    do {
        client cli(this, timeout, securefds);
        handlecli(cli);
    } while (true);
}

/**
 * Metoda za asinkdorni rad s klijentima, prima limit, pokazivač na funkciju i timeout;
 * Funkcija handlecli prima referencu tipa client - važno za definiranje funkcija koje se šalju;
 * Nije moguće proslijediti druge parametre;
*/

void server::async(const uint limit, void (*handlecli)(client&, mutex&), const uint timeout) {
    mutex io;
    do {
        for (uint i=0; i<limit; i++) {
            thr.push_back(thread([&](){
                client cli(this, timeout, securefds);
                handlecli(cli, io);
            }));
        }

        for (uint i=0; i<limit; i++) {
            thr[i].join();
        }
        thr.clear();

    } while (true);
}


/**
 * Destruktor varijable tipa server
*/


server::~server () {

    if (sock<=0) {
        throw string("[ERROR] The socket is already closed "); 
    }

    else {

        #if __linux__
            if (close(sock) != 0) {
                throw string("[ERROR] Unable to close socket ");
            }
        #elif _WIN32
            if (closesocket(sock) != 0) {
                throw string("[ERROR] Unable to close socket ");
            }
            WSACleanup();
        #endif


    }
}

/**
 * Konstruktor varijable tipa secure za klijentske implementacije
*/

secure::secure() {

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    fds = SSL_CTX_new(SSLv23_client_method());
    if (!fds) {
        throw string("[ERROR] Creating SSL context ");

    }
}


/**
 * Konstruktor varijable tipa secure za serverske implementacije
*/

secure::secure(const string cert, const string priv) {

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    // Create an SSL context
    fds = SSL_CTX_new(SSLv23_server_method());
    if (!fds) {
        throw string("[ERROR] Creating SSL context ");
    }

    // Load the server's certificate and private key files
    if (SSL_CTX_use_certificate_file(fds, cert.c_str(), SSL_FILETYPE_PEM) <= 0) {
        throw string("[ERROR] Loading certificate file ");
    }

    if (SSL_CTX_use_PrivateKey_file(fds, priv.c_str(), SSL_FILETYPE_PEM) <= 0) {
        throw string("[ERROR] Loading private key file ");
    }

}

/**
 * Destruktor varijable tipa secure
*/


secure::~secure () {
    SSL_CTX_free(fds);
}

/**
 * Konstruktor varijable tipa client
 * Prima adresu, port, i opcijonalno dozvoljeno vrijeme čekanja servera
 * i deskriptor datoteke SSL certifikat za sigurne komunikacije
*/

client::client(const string address, const ushort port, const uint timeout, SSL_CTX* securefds) {

    #if _WIN32
        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
            throw string("[ERROR] Unable to set WinSock " + to_string(WSAGetLastError()));
        }
    #endif

    conn = socket(AF_INET, SOCK_STREAM, 0);
	if (conn < 0) {
        throw string("[ERROR] Unable to open TCP socket ");
	}

    const string _address = isIPAddress(address) ? address : ipFromDomain(address);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(_address.c_str());
	addr.sin_port = htons(port);

    if (connect(conn, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) != 0) {
        throw string("Unable to connect to server ");
    }

    #if __linux__
        struct timeval tv;
        tv.tv_sec = timeout/1000;
        tv.tv_usec = (timeout%1000)*1000;

        if (setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval))) {
            throw string("[ERROR] Unable to set timeout ");
        }
    #elif _WIN32
        DWORD tv = timeout;
        if (setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv))) {
            throw string("[ERROR] Unable to set timeout ");
        }
    #endif

    if (securefds) {
        ssl = SSL_new(securefds);
        if (!ssl) {
            throw string("[ERROR] Creating SSL object ");
        }
        SSL_set_fd(ssl, conn);

        // Perform the SSL handshake
        if (SSL_connect(ssl) <= 0) {
            SSL_free(ssl);
            throw string("[ERROR] Performing SSL handshake ");
        }
    }
    

}

/**
 * Konstruktor varijable tipa commint
 * Prima pokazivač na inicijaliziranu varijablu tipa, port, 
 * i opcijonalno dozvoljeno vrijeme čekanja servera i deskriptor datoteke
 * SSL certifikat za sigurne komunikacije
*/


client::client(const server *_srv, const uint timeout, SSL_CTX* securefds) {
    srv = _srv;
    socklen_t len = sizeof(struct sockaddr_in);

    if ((conn = accept(srv->sock, (struct sockaddr *)&(srv->addr), (socklen_t*)&len)) < 0) {
        throw string("[ERROR] Unable to accept client connection ");
    }

    #if __linux__
        struct timeval tv;
        tv.tv_sec = timeout/1000;
        tv.tv_usec = (timeout%1000)*1000;

        if (setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval))) {
            throw string("[ERROR] Unable to set timeout ");
        }
    #elif _WIN32
        DWORD tv = timeout;

        if (setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv))) {
            throw string("[ERROR] Unable to set timeout ");
        }
    #endif

    if (securefds) {
        ssl = SSL_new(securefds);
        if (!ssl) {
            throw string("[ERROR] Creating SSL object ");
        }
        SSL_set_fd(ssl, conn);

        // Perform SSL handshake
        if (SSL_accept(ssl) <= 0) {
            SSL_free(ssl);
            throw string("[ERROR] Performing SSL handshake ");
        }
    }    

    char ipv4_buff[INET_ADDRSTRLEN];
    char ipv6_buff[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET, &(srv->addr.sin_addr), ipv4_buff, INET_ADDRSTRLEN);
    ipv4 = ipv4_buff;
    inet_ntop(AF_INET6, &(srv->addr.sin_addr), ipv6_buff, INET6_ADDRSTRLEN);
    ipv6 = ipv6_buff;


}


/**
 * Destruktor varijable tipa client
*/

client::~client () {

    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }

    if (conn <= 0) {
        throw string("[ERROR] The socket is already closed "); 
    }

    else {
        #if __linux__
            if (close(conn) != 0) {
                throw string("[ERROR] Unable to close socket ");
            }
        #elif _WIN32
            if (closesocket(conn) != 0) {
                throw string("[ERROR] Unable to close socket ");
            }
            //WSACleanup();
        #endif
    }

}

/**
 * Metoda klase client za slanje podataka preko soketa
 * Prima string koji će biti poslan
 * Vraća logički statu poređenja psolanih karaktera i karaktera u stringu
*/


bool client::push (const string msg) {
    size_t sended = 0;
    if (ssl) {
       sended = SSL_write(ssl, msg.c_str(), msg.length());
    }
    else {
        sended = send(conn, msg.c_str(), msg.length(), 0);
    }
    return sended == msg.length();
}

/**
 * Metoda klase client za primanje poruke preko soketa
 * Prima dozvoljeni broj karaktera koji će primiti
 * Vraća string primljene poruke
*/

string client::pull (size_t byte_limit) {
   char res[byte_limit] = {0};

    if (ssl) {
        SSL_read(ssl, res, byte_limit);
    }
    else {
        recv(conn , res, byte_limit, 0);
    }

    return string(res);
}



queue::queue (const uint _limit, const string _address, const ushort _port, const uint _timeout, SSL_CTX* _securefds) {

    if (_limit > 1) {
        limit = _limit;
    }
    else {
        throw string("[ERROR] Limit out of range ");
    }

    address = _address;
    port = _port;
    timeout = _timeout;
    securefds = _securefds;
    

    // for (uint i=0; i<limit; i++) {

    //     if (address.empty() && srv != NULL) {
    //         clients.push_back(new client(srv, timeout, securefds));
    //     }
    //     else {
    //         clients.push_back(new client(address, port, timeout, securefds));
    //     }
    // }

    // controller = thread([&](){
    //     cout << "Upao u tred "<< endl;
    //     while(true) {
    //         // io.lock();
    //         // uint missing = limit-clients.size();
    //         // cout << "nedostaje " << missing << endl;
    //         // if (missing > 0.6*limit) {
    //             // enqueue();
    //         if (clients.size() <= 1) {
    //             if (address.empty() && srv != NULL) {
    //                 clients.push_back(new client(srv, timeout, securefds));
    //             }
    //             else {
    //                 clients.push_back(new client(address, port, timeout, securefds));
    //             }
    //         }
    //         // }
    //         // usleep(missing*100000);
    //         // io.unlock();
    //         // usleep(1000);
    //         sleep(1);

    //     }
    // });

}



queue::queue (const uint _limit, server *_srv, const uint _timeout, SSL_CTX* _securefds) {
    if (_limit > 1) {
        limit = _limit;
    }
    else {
        throw string("[ERROR] Limit out of range ");
    }

    srv = _srv;
    timeout = _timeout;
    securefds = _securefds;

    controller = thread([this](){
        cout << "Upao u tred "<< endl;
        while(true) {
            io.lock();
            // uint missing = limit-clients.size();
            // cout << "nedostaje " << missing << endl;
            // if (missing > 0.6*limit) {
                // enqueue();
            if (clients.size()< limit) {
                if (address.empty() && srv != NULL) {
                    clients.push_back(new client(srv, timeout, securefds));
                }
                else {
                    clients.push_back(new client(address, port, timeout, securefds));
                }
            }
            // }
            // usleep(missing*100000);
            io.unlock();
            // usleep(1000);
        }
    });

}

void queue::enqueue () {
    vector<thread> worker;
    lock_guard<mutex> master(io);

    for (uint i=0; i<limit-clients.size(); i++) {
        cout << "idemo dodati klijente " << endl;
        worker.push_back(thread([&](){
            if (address.empty() && srv != NULL) {
                clients.push_back(new client(srv, timeout, securefds));
            }
            else {
                cout << "dodajem " << i << endl;
                clients.push_back(new client(address, port, timeout, securefds));
            }
        }));
    }

    for (uint i=0; i<worker.size(); i++) {
        cout << "čekam da se dodaju " << endl;
        worker[i].join();
    }
    
    cout << "dodali su se " << endl;
    worker.clear();
}

client queue::dequeue() {
    cout << "uzimam jednog iz stacka " << endl;
    lock_guard<mutex> master(io);
    client *cli;
    while (true) {
        if (clients.size() >= 1) {
            cli = clients.front();
            clients.pop_front();
            cout << "uzimam jednog iz stacka uzeo sam " << endl;
            return *cli;
        }
        else if (clients.size() == 1) {
             controller = thread([&](){
                cout << "Upao u tred "<< endl;
                while(true) {
                    // io.lock();
                    // uint missing = limit-clients.size();
                    // cout << "nedostaje " << missing << endl;
                    // if (missing > 0.6*limit) {
                        // enqueue();
                    if (clients.size() <= 1) {
                        if (address.empty() && srv != NULL) {
                            clients.push_back(new client(srv, timeout, securefds));
                        }
                        else {
                            clients.push_back(new client(address, port, timeout, securefds));
                        }
                    }
                    // }
                    // usleep(missing*100000);
                    // io.unlock();
                    // usleep(1000);
                    sleep(1);

                }
            });
        }

        usleep(1000);
    }
}

queue::~queue () {
    for (uint i=0; i<clients.size(); i++) {
        clients[i]->~client();
    }
    clients.clear();
}