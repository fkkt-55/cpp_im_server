// Created by Fkkt on 2023/9/7.
//
#include <server.h>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>

#ifdef __linux__
#include <arpa/inet.h>
#endif  // __linux__

namespace fkkt {
server::server(const std::string& address, int port) {
    this->address = address;
    this->port = port;
    this->log = std::move(std::make_unique<fkkt::logger>(__FUNCTION__));
    this->log->set_console_log_level(logger::D);
}

server::~server() {
#ifdef _WIN32
    if (client_socket) {
        closesocket(client_socket);
    }
    WSACleanup();
#endif  // _WIN32
}

void server::start() {
#ifdef __linux__
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        log->e("Failed to create socket");
        return;
    }
#endif  // __linux__

#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        log->e("Failed to initialize Win_sock");
        return;
    }
#endif  // _WIN32

    // Set server info
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(address.data());
    serverAddr.sin_port = htons(port);

    // Connect to server
    if (connect(client_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) ==
        -1) {
        log->e("Can't connect to server");
        return;
    }

    log->i("Connected to server");

    char message[1024];

    while (true) {
        std::cout << "Enter a message (or 'exit' to quit): ";
        std::cin.getline(message, sizeof(message));

        if (strcmp(message, "exit") == 0) {
            break;
        }

        send(client_socket, message, strlen(message), 0);

        memset(message, 0, sizeof(message));
        int bytesReceived = recv(client_socket, message, sizeof(message), 0);

        if (bytesReceived <= 0) {
            log->w("Connection closed by server");
            break;
        }

        std::cout << "Server replied: " << message << std::endl;
    }
}
}  // namespace fkkt
