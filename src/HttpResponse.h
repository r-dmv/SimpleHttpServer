#pragma once


#include <string>
#include <unordered_map>
#include <stddef.h>

using std::string;
using std::unordered_map;

#define HTTP_CODE_OK 200
#define HTTP_CODE_NOT_FOUND 404
#define HTTP_CODE_BAD_REQUEST 400
#define HTTP_CODE_FORBIDDEN 403

class HttpResponse {
public:
    HttpResponse();

    void setContentLength(size_t contentLength);
    void setContentType(string &contentType);
    void setStatusCode(int status);

    size_t getHeaderLength();
    string getRawHeader();
    int getStatusCode();

    static std::unordered_map<int, string> const statusCodes;

private:
    int status;
    size_t contentLength;
    string contentType;
    string connection;
    string date;
    size_t headerLength;
};
