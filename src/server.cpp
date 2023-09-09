//
// Created by Fkkt on 2023/9/7.
//

#include <iostream>
#include <utility>
#include <server.h>

namespace fkkt {
    server::server(const std::string &address, int port) {
        this->address = address;
        this->port = port;
        this->log = std::move(std::make_unique<fkkt::logger>(__FUNCTION__));
        this->log->set_console_log_level(logger::D);
    }

    server::~server() {
        if (client_socket) {
            closesocket(client_socket);
        }
        WSACleanup();
    }

    void server::start() {
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            log->e("Failed to initialize Win_sock");
            return;
        }

        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == INVALID_SOCKET) {
            log->e("Failed to create socket");
            return;
        }

        // Set server info
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(address.data());
        serverAddr.sin_port = htons(port);

        // Connect to server
        if (connect(client_socket, (sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            log->e("Can't connect to server");
            return;
        }

        log->i("Connected to server");

        char message[1024];

        // 客户端发送消息并接收回复
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
}
