#pragma once

#include <memory>
#include <map>
#include <unistd.h>
#include <ev++.h>
#include <syslog.h>
#include <netinet/in.h>



#define INF(FMT,...) syslog(LOG_INFO, "[INF] " FMT , ## __VA_ARGS__)
#define WRN(FMT,...) syslog(LOG_WARNING, "[WARNNING] %s(%d): " FMT , __func__, __LINE__, ## __VA_ARGS__)
#define ERR(FMT,...) syslog(LOG_ERR, "[ERROR] %s(%d): " FMT , __func__, __LINE__, ## __VA_ARGS__)

namespace ttm {

struct Client {
    int fd;
    char buffer[256];
    ssize_t data_size;
    struct sockaddr_in addr;
    socklen_t addr_len;
    std::unique_ptr<ev::io> io;
};

using Clients = std::map<int, Client>;

} // namespace ttm
