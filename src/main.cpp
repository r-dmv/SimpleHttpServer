#include <iostream>
#include "server.h"


#include "utils.h"
#include <iostream>

int main() {
    std::cout << "Starting SimpleHttpServer..." << std::endl;

    Server *newServer = new Server();
    newServer->start();

    delete newServer;
    return 0;
}