#include "server.h"

#include <unistd.h>
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <arpa/inet.h>


void Server::read(struct bufferevent *bev, void *ctx) {
    evbuffer *input = bufferevent_get_input(bev);
    evbuffer *output = bufferevent_get_output(bev);

    //std::cout << evbuffer_pullup(input, evbuffer_get_length(input)) << std::endl;
    evbuffer_add(output,
            (void *) "HTTP/1.1 200 OK\r\nServer: nginx/1.2.4\r\nDate: Sun, 14 Sep 2014 18:35:55 GMT\r\nContent-Type: text/html; charset=windows-1251\r\nContent-Length: 0\r\nConnection: close\r\n\r\n",
            strlen(  "HTTP/1.1 200 OK\r\nServer: nginx/1.2.4\r\nDate: Sun, 14 Sep 2014 18:35:55 GMT\r\nContent-Type: text/html; charset=windows-1251\r\nContent-Length: 0\r\nConnection: close\r\n\r\n"));

}

void Server::dropConnection(struct bufferevent *bev, void *ctx) {
    std::cout << "Write" << std::endl;
}

void Server::eventCallback(struct bufferevent *bev, short events, void *ctx) {
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
        std::cout << "Socket closed" << std::endl;
    }
}

void Server::acceptConnection(struct evconnlistener *listener,
        evutil_socket_t fd, struct sockaddr *address, int socketLength,
        void *ctx) {
    event_base *base = evconnlistener_get_base(listener);
    bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, read, dropConnection, eventCallback, NULL);

    bufferevent_enable(bev, EV_READ);
}

void Server::acceptError(struct evconnlistener *listener, void *ctx) {
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));

    event_base_loopexit(base, NULL);
}

void Server::start() {
    std::cout << "Parent PID: " << getpid() << std::endl;

    struct event_base *base;
    struct evconnlistener *listener;
    struct sockaddr_in sin;

    int port = 8010;


    base = event_base_new();
    if (!base) {
        puts("Couldn't open event base");
        return;
    }

    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(0);
    sin.sin_port = htons(port);

    listener = evconnlistener_new_bind(base, acceptConnection, NULL,
            LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
            (struct sockaddr*)&sin, sizeof(sin));

    if (!listener) {
        perror("Couldn't create listener");
        return;
    }
    evconnlistener_set_error_cb(listener, acceptError);

    numberOfWorkers = sysconf( _SC_NPROCESSORS_ONLN ) + 1;

    for (long i = 0; i < numberOfWorkers; i++) {
        auto pid = fork();

        if (!pid) {
            event_reinit(base);
            break;
        }
    }


    event_base_dispatch(base);

}