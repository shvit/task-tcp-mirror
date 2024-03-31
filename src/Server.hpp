#pragma once

#include "Common.hpp"

namespace ttm {

class Server {
protected:
    int fd_main{-1}; ///< Main listening descriptor
    Clients clients_list; ///< Clients list

    /** @brief Callback on adding client
    */
    void add_client_cb(ev::io &w, int revents);

    /** @brief Callback on client data ready
    */
    void client_data_cb(ev::io &w, int revents);

    /** @brief Openning socket and binding and listenong
     * 
     * @param [in] host Listening address
     * @param [in] port Listenong port
     * @return true on success else false
    */
    bool listen_start(const char* host, uint16_t port);

    /** @brief Close sockets
    */
    void listen_stop();

public:

    /** @brief Start server
     * 
     * @param [in] host Listening address
     * @param [in] port Listenong port
     * @return false on initialize error else true
    */
    bool run(const char* host, uint16_t port);
};


} // namespace ttm
