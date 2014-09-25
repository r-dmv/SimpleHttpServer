#include <iostream>
#include "server.h"


int main() {
    std::cout << "Starting SimpleHttpServer..." << std::endl;

    Server *newServer = new Server();
    newServer->start();


    delete newServer;
    return 0;
}