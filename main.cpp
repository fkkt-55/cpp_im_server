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

#ifdef _WIN32
    system("pause");
#endif // _WIN32
    return 0;
}

