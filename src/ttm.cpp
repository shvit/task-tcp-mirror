#include <cstdlib>
#include <iostream>

#include "Server.hpp"
#include "Common.hpp"

int main(int argc, char* argv[]) {
    int ret = EXIT_SUCCESS;
    openlog("ttm", LOG_NDELAY, LOG_DAEMON | LOG_PID);
    INF("Run TCP message mirror server app");

    ttm::Server srv;
    if (!srv.run("0.0.0.0", 5000U)) {
        std::cerr << "Server can't started. See system log for delails" << std::endl;
        ret = EXIT_FAILURE;
    }

    INF("Exit TCP message mirror server app");
    closelog();

    return ret;
}
