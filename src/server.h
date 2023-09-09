//
// Created by Fkkt on 2023/9/7.
//

#ifndef CPP_IM_SERVER_SERVER_H
#define CPP_IM_SERVER_SERVER_H

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <logger.h>

#ifdef _WIN32
#include <winsock2.h>
#endif // _WIN32

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif // __linux__

namespace fkkt {
    class server {
    public:
        server(const std::string &address, int port);

        ~server();

        void start();

    private:
        std::unique_ptr<fkkt::logger> log = nullptr;
        std::string address;
        int port;
#ifdef _WIN32
        WSAData wsa_data{};
        SOCKET client_socket{};
#endif
#ifdef __linux__
        // WSAData wsa_data{};
        int client_socket{};
#endif
    };
}


#endif //CPP_IM_SERVER_SERVER_H
