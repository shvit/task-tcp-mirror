#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include "Server.hpp"

namespace ttm {

bool Server::listen_start(const char* host, uint16_t port) {
   fd_main = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd_main < 0) {
        ERR("Failed create socket");
        return false;
    }

    if (int opt=1; setsockopt(fd_main, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt))) {
        ERR("Failed set socket option reuse addr");
        listen_stop();
        return false;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = inet_addr(host);
    if(bind(fd_main, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        ERR("Binding failed on %s:%u", host, port);
        listen_stop();
        return false;
    }

    if(listen(fd_main, 100)) {
        ERR("Listening failed on %s:%u", host, port);
        listen_stop();
        return false;
    }

    return true;
}

void Server::listen_stop() {
    close(fd_main);
    fd_main = -1;
}

void Server::client_data_cb(ev::io &w, int revents) {
    auto cl = clients_list.find(w.fd);
    if (cl != clients_list.end()) {
        cl->second.data_size = recvfrom(cl->second.fd, cl->second.buffer, sizeof(cl->second.buffer) - 1, 0, (struct sockaddr*) &cl->second.addr, (socklen_t *) &cl->second.addr_len);
        if (cl->second.data_size > 0) {
            cl->second.buffer[cl->second.data_size] = '\0';
            INF("RX from client (fd=%d): %ld bytes: '%s'", cl->second.fd, cl->second.data_size, cl->second.buffer);
            sendto(cl->second.fd, cl->second.buffer, cl->second.data_size, 0, (struct sockaddr*) &cl->second.addr, cl->second.addr_len);
        } else {
            close(w.fd);
            cl->second.io->stop();
            clients_list.erase(cl);
            INF("Client (fd=%d) disconnected. Left %lu clients", w.fd, clients_list.size());
        }
    }
}

void Server::add_client_cb(ev::io &w, int revents) {
    struct sockaddr_in addr;
    socklen_t addr_len;
    int temp_fd = accept(fd_main, (struct sockaddr *)&addr, &addr_len);
    if (temp_fd < 0) {
        return;
    }
    auto [iter, flag] = clients_list.emplace(temp_fd, Client{});
    iter->second.fd = temp_fd;
    iter->second.addr = addr;
    iter->second.addr_len = addr_len;
    iter->second.io = std::make_unique<ev::io>();
    iter->second.io->set <Server, &Server::client_data_cb>(this);
    iter->second.io->set(temp_fd, ev::READ);
    iter->second.io->start();
    INF("Connected new client (%lu) with fd=%d", clients_list.size(), temp_fd);
}

bool Server::run(const char* host, uint16_t port) {
    INF("Try to running server");
    if (fd_main < 0) {
        if (listen_start(host, port)) {
            INF("Successful start listening");
        } else {
            ERR("Failed start listening");
            return false;
        }
    }

    ev::io io;
    io.set <Server, &Server::add_client_cb >(this);
    io.set(fd_main, ev::READ);
    io.start();
    io.loop.run();

    return true;
}

} // namespace ttm
