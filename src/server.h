//
// Created by Fkkt on 2023/9/7.
//

#ifndef CPP_IM_SERVER_SERVER_H
#define CPP_IM_SERVER_SERVER_H

#include <logger.h>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#endif  // _WIN32

namespace fkkt {

#ifdef _WIN32
typedef SOCKET KtSocket;
#else
typedef int KtSocket;
#endif

class server {
   public:
    server(const std::string& address, int port);

    ~server();

    void start();

   private:
    std::unique_ptr<fkkt::logger> log = nullptr;
    std::string address;
    int port;
    KtSocket server_socket{};
#ifdef _WIN32
    WSAData wsa_data{};
#endif
};
}  // namespace fkkt

#endif  // CPP_IM_SERVER_SERVER_H
