#ifndef REQUESTHELPER_H
#define REQUESTHELPER_H

#include <string>

class RequestHelper {
private:
    int sockfd;

public:
    RequestHelper();
    ~RequestHelper();

    std::string sendRequest(const std::string& request);
};

#endif // REQUESTHELPER_H
