#include <server.h>
#include <memory>
#include <string>
#include <logger.h>

using namespace std;

int main() {
    string address = "127.0.0.1";
    int port = 10924;

    auto server = std::make_unique<fkkt::server>(address, port);
    server->start();

    system("pause");

    return 0;
}

