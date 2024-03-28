#pragma once

#include <list>
#include <unistd.h>
#include <ev.h>
#include <event2/util.h>
#include <event2/event.h>
#include <syslog.h>


#define INF(FMT,...) syslog(LOG_INFO, "[INF] " FMT , ## __VA_ARGS__)
#define WRN(FMT,...) syslog(LOG_WARNING, "[WARNNING] %s(%d): " FMT , __func__, __LINE__, ## __VA_ARGS__)
#define ERR(FMT,...) syslog(LOG_ERR, "[ERROR] %s(%d): " FMT , __func__, __LINE__, ## __VA_ARGS__)

namespace ttm {

constexpr size_t READ_BUFF_SIZE = 128U;
constexpr size_t WRITE_BUFF_SIZE = 128U;

struct ConnCtx {
    evutil_socket_t fd;
    struct event_base* base{nullptr};
    struct event* read_event{nullptr};
    struct event* write_event{nullptr};
    uint8_t read_buff[READ_BUFF_SIZE];
    uint8_t write_buff[WRITE_BUFF_SIZE];
    ssize_t read_buff_used{0U};
    ssize_t write_buff_used{0U};
};

using ConnList = std::list<ConnCtx>;


} // namespace ttm
