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
    if (server_socket) {
        closesocket(server_socket);
    }
    WSACleanup();
#endif  // _WIN32
}

void server::start() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        log->e("Failed to create socket");
        return;
    }

#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        log->e("Failed to initialize Win_sock");
        return;
    }
#endif  // _WIN32

    // Set server info
    struct sockaddr_in serverAddr {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        log->e("Binding socket failed");
        return;
    }

    if (listen(server_socket, 5) == -1) {
        log->e("Error listening");
        return;
    }

    log->i("Server listening on port ", port);

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);
        KtSocket client_socket =
            accept(server_socket, (sockaddr*)&client_socket, &clientAddrLen);
        if (client_socket == -1) {
            log->e("Error while accepting connection");
            break;;
        }

        // commuicate with client
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = recv(client_socket, buffer, sizeof(buffer), 0)) >
               0) {
            buffer[bytesRead] = '\0';
            log->d("recvived: ", buffer);

            // replay
            auto response = "Hello, client";
            send(client_socket, response, strlen(response), 0);
        }
        if (bytesRead == -1) {
            log->e("Error receiving data");
        }
    }
}
}  // namespace fkkt
