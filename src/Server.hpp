#pragma once

//#include <atomic>

#include "Common.hpp"

namespace ttm {

class Server {
protected:
    ConnList lst;
    
public:
    void run(const char* host, uint16_t port);

};

} // namespace ttm
