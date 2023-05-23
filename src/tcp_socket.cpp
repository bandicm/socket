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
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        printf("[ERROR] Ne mogu otvoriti defenirani TCP/IP socket!");
    }

    if (bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
        printf("[ERROR] Ne mogu bindati defenirani TCP/IP socket!");
    }

    if (listen(sock, limit) < 0) {
        printf("[ERROR] Ne mogu defenirani listen za TCP/IP socket!");
    }

}


server::~server () {

    if (sock<=0) {
        printf ("[ERROR] Soket destruktor: već zatvoren soket!");
    }

    else if (close(sock) != 0) {
        printf ("[ERROR] Soket destruktor nije mogao zatvoriti soket!");
    }

}


secure::secure() {
    fds = SSL_CTX_new(SSLv23_client_method());
    if (!fds) {
        //throw "[ERROR] Creating SSL context ";
        cout << endl << "[ERROR] Creating SSL context ";

    }
}

secure::secure(const string cert, const string priv) {

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

        // Create an SSL context
    fds = SSL_CTX_new(SSLv23_server_method());
    if (!fds) {
        // throw "[ERROR] Creating SSL context ";
        cout << endl << "[ERROR] Creating SSL context ";
    }

    // Load the server's certificate and private key files
    if (SSL_CTX_use_certificate_file(fds, cert.c_str(), SSL_FILETYPE_PEM) <= 0) {
        // throw "[ERROR] Loading certificate file.";
        cout << endl << "[ERROR] Loading certificate file.";
    }

    if (SSL_CTX_use_PrivateKey_file(fds, priv.c_str(), SSL_FILETYPE_PEM) <= 0) {
        //throw "[ERROR] Loading private key file.";
        cout << endl << "[ERROR] Loading private key file.";
    }

}

secure::~secure () {
    SSL_CTX_free(fds);
}


client::client(const string address, const ushort port, const uint timeout, SSL_CTX* securefds) {

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

    struct timeval tv;
    tv.tv_sec = 0;		// za sad 2 sekunde timeout, harkodirano
    tv.tv_usec = timeout*1000;

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval))) {
        printf ("[ERROR] 503 Ne mogu postaviti timeout!");
		sock = -1;
    }

    if (securefds) {
        ssl = SSL_new(securefds);
        if (!ssl) {
            //throw "[ERROR] Creating SSL object.";
            cout << endl <<  "[ERROR] Creating SSL object.";
        }
        SSL_set_fd(ssl, sock);

    }
        // Perform the SSL handshake
    if (SSL_connect(ssl) <= 0) {
        SSL_free(ssl);
        // throw "[ERROR] Performing SSL handshake.";
        cout << endl << "[ERROR] Performing SSL handshake.";
    }

}


client::~client () {

    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }

    if (sock <= 0) {
        printf ("[ERROR] Soket destruktor: već zatvoren soket!");
    }

    else if (close(sock) != 0) {
        printf ("[ERROR] Soket destruktor nije mogao zatvoriti soket!");
    }

}


bool client::tell (const string msg) {
    size_t sended = 0;
    if (ssl) {
       sended = SSL_write(ssl, msg.c_str(), msg.length());
    }
    else {
        sended = write(sock, msg.c_str(), msg.length());
    }
    return sended == msg.length();
}

string client::obey (size_t byte_limit) {
   char res[byte_limit] = {0};

    if (ssl) {
        SSL_read(ssl, res, byte_limit);
    }
    else {
        read(sock , res, byte_limit);
    }

    return (string) res;
}



comming::comming(const server *_srv, const uint timeout, SSL_CTX* securefds) {
    srv = _srv;
    socklen_t len = sizeof(struct sockaddr_in);

    if ((conn = accept(srv->sock, (struct sockaddr *)&(srv->addr), (socklen_t*)&len)) < 0) {
        printf("[ERROR] Ne mogu preuzeti vezu klijenta!");
    }

    struct timeval tv;
    tv.tv_sec = 1;		// za sad 2 sekunde timeout, harkodirano
    tv.tv_usec = 0;

    if (setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval))) {
        printf("[ERROR] 503 Ne mogu postaviti timeout!");
		conn = -1;
    }


    if (securefds) {
        ssl = SSL_new(securefds);
        if (!ssl) {
            // throw "[ERROR] Creating SSL object.";
            cout << endl << "[ERROR] Creating SSL object.";
        }
        SSL_set_fd(ssl, conn);
        // Perform SSL handshake
        if (SSL_accept(ssl) <= 0) {
            SSL_free(ssl);
            // throw "[ERROR] Performing SSL handshake.";
            cout << endl << "[ERROR] Performing SSL handshake.";
        }
    }    

    char ipv4_buff[INET_ADDRSTRLEN];
    char ipv6_buff[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET, &(srv->addr.sin_addr), ipv4_buff, INET_ADDRSTRLEN);
    ipv4 = ipv4_buff;
    inet_ntop(AF_INET6, &(srv->addr.sin_addr), ipv6_buff, INET6_ADDRSTRLEN);
    ipv6 = ipv6_buff;


}

comming::~comming() {

    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }

    if (conn <= 0) {
        printf ("[ERROR] Comming destruktor: već zatvoren soket!");
    }

    else if (close(conn) != 0) {
        printf ("[ERROR] Comming destruktor nije mogao zatvoriti soket!");
    }
}

bool comming::tell (const string msg) {
    ssize_t sended = 0;
    if (ssl) {
       sended = SSL_write(ssl, msg.c_str(), msg.length());
    }
    else {
        sended = write(conn, msg.c_str(), msg.length());
    }
    return sended == msg.length();
}

string comming::obey (size_t byte_limit) {
    char res[byte_limit] = {0};

    if (ssl) {
        SSL_read(ssl, res, byte_limit);
    }
    else {
        read(conn , res, byte_limit);
    }

    return (string) res;
}
