#include "utils.h"
#include <unordered_map>
#include <regex>

std::unordered_map<std::string, std::string> const mimeTable = {
        {".htm", "text/html"},
        {".html", "text/html"},
        {".htmls", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".swf", "application/x-shockwave-flash"}
};

std::vector<std::shared_ptr<std::string>> splitString(unsigned char *str, size_t length) {
    std::vector<std::shared_ptr<std::string>> result;
    size_t passed = 0;

    for (size_t i = 0; i < length; i++) {
        if (str[i] == '\n' || i == length - 1) {
            std::shared_ptr<std::string> ptr(new std::string(str + passed, str + i - 1));
            passed = i + 1;
            result.push_back(ptr);
        }
    }

    return result;
}

std::string getMimeType(std::string fileName) {
    std::regex  fileExtensionRegex("(\\.[^.]+)$");
    std::smatch fileExtension;

    std::regex_search(fileName, fileExtension, fileExtensionRegex);

    if (fileExtension.size() > 1) {
        try {
            return mimeTable.at(fileExtension[1]);
        } catch(...) {
            return "text/html";
        }
    }
    return "text/html";
}