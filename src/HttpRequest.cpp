#include "HttpRequest.h"
#include <regex>
#include "utils.h"
#include <iostream>

using std::smatch;
using std::regex_search;

regex const HttpRequest::firstLinePattern = regex("(GET|PUT|POST|DELETE|HEAD)\\s([^\\?]*)\\?{0,1}(.*)\\s(\\S+)");

HttpRequest::HttpRequest(unsigned char *rawRequest, size_t length) {
    auto requestRows = splitString(rawRequest, length);

    if (requestRows.size() < 1) {
        throw BadRequestException();
    }

    smatch requestLine;
    if (regex_search(*(requestRows[0].get()), requestLine, HttpRequest::firstLinePattern) && requestLine.size() > 3) {
        this->method = requestLine[1];
        this->path = UriDecode(requestLine[2]);

        this->httpVersion = requestLine[4];
    } else {
        throw BadRequestException();
    }

}

string HttpRequest::getMethod() {
    return method;
}

string HttpRequest::getPath() {
    return path;
}

string HttpRequest::getHttpVersion() {
    return httpVersion;
}

void HttpRequest::setMethod(string const &method) {
    HttpRequest::method = method;
}

void HttpRequest::setPath(string const &path) {
    HttpRequest::path = path;
}

void HttpRequest::setHttpVersion(string const &httpVersion) {
    HttpRequest::httpVersion = httpVersion;
}

