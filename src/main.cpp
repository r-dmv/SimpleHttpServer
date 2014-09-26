#include <iostream>
#include "server.h"


int main() {
    std::cout << "Starting SimpleHttpServer..." << std::endl;

    Server *newServer = new Server();
    newServer->start();

    std::cout << "Shutdown SimpleHttpServer..." << std::endl;
    delete newServer;
    return 0;
}