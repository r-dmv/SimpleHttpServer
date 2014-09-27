#include "utils.h"
#include <unordered_map>
//#include <regex>
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

const char HEX2DEC[256] = {
        /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
        /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

        /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};

std::vector<std::shared_ptr<std::string>> SplitString(unsigned char *str, size_t length) {
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

std::string GetMimeType(std::string fileName) {
    /*std::regex  fileExtensionRegex("(\\.[^.]+)$");
    std::smatch fileExtension;

    std::regex_search(fileName, fileExtension, fileExtensionRegex);

    if (fileExtension.size() > 1) {
        try {
            return mimeTable.at(fileExtension[1]);
        } catch(...) {
            return "text/html";
        }
    }*/
    return "text/html";
}

std::string UriDecode(const std::string & sSrc) {
    const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
    const long SRC_LEN = sSrc.length();
    const unsigned char * const SRC_END = pSrc + SRC_LEN;
    const unsigned char * const SRC_LAST_DEC = SRC_END - 2;

    char * const pStart = new char[SRC_LEN];
    char * pEnd = pStart;

    while (pSrc < SRC_LAST_DEC) {
        if (*pSrc == '%') {
            char dec1, dec2;
            if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
                    && -1 != (dec2 = HEX2DEC[*(pSrc + 2)])) {
                *pEnd++ = (dec1 << 4) + dec2;
                pSrc += 3;
                continue;
            }
        }

        *pEnd++ = *pSrc++;
    }

    while (pSrc < SRC_END)
        *pEnd++ = *pSrc++;

    std::string sResult(pStart, pEnd);
    delete [] pStart;
    return sResult;
}


bool IsDirectoryOutOfRoot(std::string &fileName) {
    char const *strFileName = fileName.c_str();
    int fileDeep = 0;

    std::string currentDirectory;
    for(int i = 0; i < fileName.size() && fileDeep >= 0; i++) {
        if (strFileName[i] == '/') {
            if (i != 0) {
                if (currentDirectory == "..") {
                    fileDeep--;
                } else {
                    if (currentDirectory != "" || currentDirectory != ".") {
                        fileDeep++;
                    }
                }
                currentDirectory.clear();
            }
        } else {
            currentDirectory += strFileName[i];
        }
    }

    if (fileDeep < 0) {
        return false;
    }

    return true;
}