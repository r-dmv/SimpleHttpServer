#pragma once

#include <string>
#include <regex>

using std::string;
using std::regex;

class HttpRequest {
private:
public:
    HttpRequest(unsigned char *rawRequest, size_t length);

    string getMethod();
    string getPath();
    string getHttpVersion();

    void setMethod(string const &method);
    void setPath(string const &path);
    void setHttpVersion(string const &httpVersion);

private:
    static regex const firstLinePattern;

    string method;
    string path;
    string httpVersion;
};

class BadRequestException {};
class Forbidden {};
class NotFound{};