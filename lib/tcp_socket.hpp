#ifndef TCP_SCK
#define TCP_SCK

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#if __linux__
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
#elif _WIN32
    #include <WinSock.h>
    #include <ws2tcpip.h>
    #pragma comment(lib,"ws2_32.lib")
    #define ushort u_short
    #define uint u_int
#endif

#include "ip.hpp"

using namespace std;

class client;
// class secure;
// class server;


/**
 * Server klasa za TCP/IP soket
 * Instanca se incijalizira kada pokrećemo server
*/

class server {
    public:
    #if __linux__
        int sock;
    #elif _WIN32
        WSADATA wsa;
        SOCKET sock;
    #endif
    struct sockaddr_in addr;
    SSL_CTX* securefds = NULL;
    vector<thread> thr;

    server (const ushort port, const uint queue = 1000, SSL_CTX* _securefds = NULL);
    ~server ();

    void sync(void (*handlecli)(client&), const uint timeout = 100);
    void async(const uint limit, void (*handlecli)(client&, mutex&), const uint timeout = 100);
    void pool(const uint limit, void (*handlecli)(client&, mutex&), const uint timeout = 100);

};

/**
 * Klasa za konstrukciju i destrukciju SSL certifikata
 * Pokreće se za server i client - (različiti konstruktori)
*/

class secure {
    public:
    SSL_CTX* fds;

    secure(); // client
    secure(const string cert, const string priv);   //server
    ~secure();

};


/**
 * Klasa za definiranje TCP soketa kod klijentskih aplikacija
*/

class client {
    public:
    // zajedničke
    #if __linux__
        int conn; // mijenja sock
    #elif _WIN32
        WSADATA wsa;
        SOCKET conn; // mijenja sock
    #endif
    struct sockaddr_in addr;
    SSL* ssl = NULL;
    // server s klijentima
    const server* srv;
    // klijent sa serverom
    string ipv4;
    string ipv6;
    
    // konstruktor za klijente bez servera
    client (const string address, const ushort port, const uint timeout = 100, SSL_CTX* securefds = NULL);
    // konstruktor za klijente sa serverom
    client (const server *_srv, const uint timeout = 100, SSL_CTX* securefds = NULL);
    ~client ();
    bool push (const string msg);
    string pull (size_t byte_limit = 1024);

    /**
     * ustvari ne znam jel konekcija aktivna
     * kod za connect i disconnect je konstruktoru - destruktoru
    */
};


class Pool {
    public:
    uint numcli;
    vector<pair<mutex*, client*>> drops;

    // konstruktor za klijente bez servera
    Pool (const uint _numcli, const string address, const ushort port, const uint timeout = 100, SSL_CTX* securefds = NULL);
    // konstruktor za klijente sa serverom
    Pool (const server *_srv, const uint _numcli, const uint timeout = 100, SSL_CTX* securefds = NULL);
    ~Pool();

    void run();

};

#endif