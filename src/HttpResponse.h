#pragma once


#include <string>
#include <unordered_map>
#include <stddef.h>

using std::string;
using std::unordered_map;

#define HTTP_CODE_OK 200
#define HTTP_CODE_NOT_FOUND 404
#define HTTP_CODE_BAD_REQUEST 405

class HttpResponse {
public:
    HttpResponse(int status);

    void setContentLength(size_t contentLength);
    void setContentType(string &contentType);

    size_t getHeaderLength();

    string getRawHeader();

    static std::unordered_map<int, string> const statusCodes;

private:
    int status;
    size_t contentLength;
    string contentType;
    string connection;
    string date;
    size_t headerLength;
};
