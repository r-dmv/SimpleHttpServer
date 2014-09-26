#include "server.h"
#include "HttpRequest.h"
#include "utils.h"
#include "HttpResponse.h"

#include <unistd.h>
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

std::string Server::rootDirectory = "/Users/dmitry/Documents/SimpleHttpServer/test";

void Server::read(bufferevent *bev, void *ctx) {
    evbuffer *input = bufferevent_get_input(bev);
    evbuffer *output = bufferevent_get_output(bev);

    HttpResponse response;
    bool sendFile = true;
    int fileDescriptor = 0;
    struct stat fileStat;

    try {
        size_t inputSize = evbuffer_get_length(input);
        HttpRequest request(evbuffer_pullup(input, inputSize), inputSize);

        string fileName = rootDirectory + request.getPath();
        string requestMethod = request.getMethod();
        bool useIndex = false;

        if (requestMethod != "GET" && requestMethod != "HEAD") {
            throw BadRequestException();
        }

        if (fileName.back() == '/') {
            fileName = fileName + INDEX_FILE;
            useIndex = true;
        }

        if (!IsDirectoryOutOfRoot(fileName)) {
            throw Forbidden();
        }

        fileDescriptor = open(fileName.c_str(), O_RDONLY | O_NONBLOCK);

        if (fileDescriptor != FILE_NOT_FOUND) {
            fstat(fileDescriptor, &fileStat);

            *((int *) ctx) = fileDescriptor;
            string contentType = GetMimeType(fileName);

            response.setStatusCode(HTTP_CODE_OK);
            response.setContentLength((size_t) fileStat.st_size);
            response.setContentType(contentType);

            if (requestMethod == "HEAD") {
                sendFile = false;
            }

        } else {
            if (useIndex) {
                throw Forbidden();
            }

            throw NotFound();
        }

    } catch (BadRequestException &e) {
        response.setStatusCode(HTTP_CODE_BAD_REQUEST);

        sendFile = false;
    } catch (Forbidden &e) {
        response.setStatusCode(HTTP_CODE_FORBIDDEN);

        sendFile = false;
    } catch (NotFound &e) {
        response.setStatusCode(HTTP_CODE_NOT_FOUND);

        sendFile = false;
    }

    string responseHeader = response.getRawHeader();

    evbuffer_add(output,
            (void *) responseHeader.c_str(),
            response.getHeaderLength());

    if (sendFile) {
        evbuffer_add_file(output, fileDescriptor, 0, fileStat.st_size);
    }

}

void Server::dropConnection(bufferevent *bev, void *ctx) {
    bufferevent_free(bev);

    close(*((int *)ctx));
    delete (int *)ctx;
}

void Server::eventCallback(bufferevent *bev, short events, void *ctx) {
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
    }
}

void Server::acceptConnection(evconnlistener *listener,
        evutil_socket_t fd, sockaddr *address, int socketLength,
        void *ctx) {
    event_base *base = evconnlistener_get_base(listener);
    bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    int *fileDescriptor = new int(0);
    bufferevent_setcb(bev, read, dropConnection, eventCallback, fileDescriptor);

    bufferevent_enable(bev, EV_READ);
}

void Server::acceptError(evconnlistener *listener, void *ctx) {
    event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));

    if (*((int *)ctx) > 0) {
        close(*((int *)ctx));
    }
    delete (int *) ctx;
    event_base_loopexit(base, NULL);
}

void Server::start() {
    event_base *base;
    evconnlistener *listener;
    sockaddr_in sin;

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
            (sockaddr*)&sin, sizeof(sin));

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