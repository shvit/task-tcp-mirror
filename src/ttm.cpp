#include <cstdlib>
//#include <iostream>

#include "Server.hpp"
#include "Common.hpp"

int main(int argc, char* argv[]) {

    openlog("ttm", LOG_NDELAY, LOG_DAEMON | LOG_PID);
    INF("Run TCP message mirror server");

    ttm::Server srv;

    srv.run("0.0.0.0", 5000U);

    INF("Exit TCP message mirror server");
    closelog();

    return EXIT_SUCCESS;
}
