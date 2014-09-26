#include "HttpResponse.h"
#include "server.h"
#include <cstring>

std::unordered_map<int, string> const HttpResponse::statusCodes = {
        {HTTP_CODE_OK, "OK"},
        {HTTP_CODE_NOT_FOUND, "Not Found"},
        {HTTP_CODE_BAD_REQUEST, "Bad Request"},
        {HTTP_CODE_FORBIDDEN, "Forbidden"}
};

HttpResponse::HttpResponse() : connection("close"), contentType("text/html"), contentLength(0) {
    time_t rawTime;
    struct tm * timeInfo;
    char buffer[32];

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    strftime(buffer, 32, "%a, %d %b %Y %X %Z", timeInfo);
    this->date = string(buffer, buffer + strlen(buffer) - 1);
}

void HttpResponse::setContentLength(size_t contentLength) {
    this->contentLength = contentLength;
}

void HttpResponse::setContentType(string &contentType) {
    this->contentType = contentType;
}

string HttpResponse::getRawHeader() {
    // status
    string header = "HTTP/1.1 " + std::to_string(status);
    header +=  " " + statusCodes.at(status) + "\r\n";
    // date
    header += "Date: " + date;
    header += "\r\n";
    // server
    header += "Server: ";
    header += SERVER_NAME;
    header += " v";
    header += SERVER_VERSION;
    header += "\r\n";
    // content-length
    header += "Content-Length: " + std::to_string(contentLength);
    header += "\r\n";
    // content-type
    header += "Content-Type: " + contentType;

    /* Commented for httptest.py
    if (contentType == "text/html") {
        header += "; charset=utf-8";
    }*/

    header += "\r\n";
    // connection
    header += "Connection: " + connection;
    header += "\r\n";
    // data
    header += "\r\n";

    headerLength = header.size();

    return header;
}

size_t HttpResponse::getHeaderLength() {
    return headerLength;
}

void HttpResponse::setStatusCode(int status) {
    this->status = status;
}

int HttpResponse::getStatusCode() {
    return status;
}