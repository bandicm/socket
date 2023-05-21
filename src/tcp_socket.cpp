#include "../lib/tcp_socket.hpp"

server::server (const ushort port, const uint limit) {

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0); 
    if (sock <= 0) {  
        printf("[ERROR] Ne mogu otvoriti defenirani TCP/IP socket!");
    }

    int opt=1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        printf("[ERROR] Ne mogu otvoriti defenirani TCP/IP socket!");
    }
    printf("[EVENT] Otvoren defenirani TCP/IP socket.");

    if (bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
        printf("[ERROR] Ne mogu bindati defenirani TCP/IP socket!");
    }

    if (listen(sock, limit) < 0) {
        printf("[ERROR] Ne mogu defenirani listen za TCP/IP socket!");
    }

}


server::~server () {

    if (sock<0) {
        printf ("[ERROR] Soket destruktor: već zatvoren soket!");
    }

    else if (!close(sock)) {
        printf ("[ERROR] Soket destruktor nije mogao zatvoriti soket!");
    }

}


client::client(const string address, const ushort port) {

    sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("[ERROR] Ne mogu otvoriti TCP/IP socket!");
	}

    const string _address = isIPAddress(address) ? address : ipFromDomain(address);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(_address.c_str());
	addr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) != 0) {
        printf("[EROR] Ne mogu se povezati s poslužiteljem!");
    }

}


client::~client () {

    if (sock <= 0) {
        printf ("[ERROR] Soket destruktor: već zatvoren soket!");
    }

    else if (close(sock) != 0) {
        printf ("[ERROR] Soket destruktor nije mogao zatvoriti soket!");
    }

}


bool client::tell (const string msg) {
    ssize_t sended = send(sock, msg.c_str(), msg.length(),0);
    return sended == msg.length();
}

string client::obey (size_t byte_limit) {
    char res[byte_limit] = {0};
    ssize_t n = read ( sock , res, byte_limit);
    return (string) res;
}



comming::comming(const server *_srv, const uint timeout) {
    srv = _srv;
    socklen_t len = sizeof(struct sockaddr_in);

    if ((conn = accept(srv->sock, (struct sockaddr *)&(srv->addr), (socklen_t*)&len)) < 0) {
        printf("[ERROR] Ne mogu preuzeti vezu klijenta!");
    }

    char ipv4_buff[INET_ADDRSTRLEN];
    char ipv6_buff[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET, &(srv->addr.sin_addr), ipv4_buff, INET_ADDRSTRLEN);
    ipv4 = ipv4_buff;
    inet_ntop(AF_INET6, &(srv->addr.sin_addr), ipv6_buff, INET6_ADDRSTRLEN);
    ipv6 = ipv6_buff;


}

comming::~comming() {
    if (conn <= 0) {
        printf ("[ERROR] Comming destruktor: već zatvoren soket!");
    }

    else if (close(conn) != 0) {
        printf ("[ERROR] Comming destruktor nije mogao zatvoriti soket!");
    }
}

bool comming::tell (const string msg) {
    ssize_t sended = send(conn, msg.c_str(), msg.length(),0);
    return sended == msg.length();
}

string comming::obey (size_t byte_limit) {
    char res[byte_limit] = {0};
    ssize_t n = read ( conn , res, byte_limit);
    return (string) res;
}