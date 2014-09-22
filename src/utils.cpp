#include "utils.h"


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