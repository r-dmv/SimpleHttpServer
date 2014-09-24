#pragma once

#include <event2/listener.h>
#include <string>

#define INDEX_FILE "index.html"
#define SERVER_NAME "SimpleHttpServer"
#define SERVER_VERSION "0.1.0"
#define FILE_NOT_FOUND -1

class Server {
public:
    Server() {};

    void start();

    static void acceptError(struct evconnlistener *listener, void *ctx);
    static void dropConnection(struct bufferevent *bev, void *ctx);
    static void acceptConnection(struct evconnlistener *listener, evutil_socket_t fd,
            struct sockaddr *address, int socketLength, void *ctx);
    static void eventCallback(struct bufferevent *bev, short events, void *ctx);
    static void read(struct bufferevent *bev, void *ctx);

protected:
    long numberOfWorkers;
    static std::string rootDirectory;

};