#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Server.hpp"

namespace ttm {

void on_read(evutil_socket_t fd, short flags, void* arg) {
    // TODO:
}

void on_write(evutil_socket_t fd, short flags, void* arg) {
    // TODO:
}

static void on_accept(evutil_socket_t listen_sock, short flags, void* arg) {
    ConnList& lst = *((ConnList*)arg);
    evutil_socket_t fd = accept(listen_sock, 0, 0);

    if(fd < 0) {
        ERR("Failed accept()");
        return;
    }
    INF("New connection fd %d", fd);

    if(evutil_make_socket_nonblocking(fd) < 0) {
        ERR("Failed evutil_make_socket_nonblocking()");
        return;
    }

    auto ctx = lst.emplace_back();
    ctx.base = lst.front().base;
    ctx.read_buff_used = 0U;
    ctx.write_buff_used = 0U;
    ctx.fd = fd;

    ctx.read_event = event_new(ctx.base, fd, EV_READ | EV_PERSIST, on_read, (void*)&ctx);
    if(!ctx.read_event) {
        ERR("Failed event_new() for EV_READ");
        return;
    }

    ctx.write_event = event_new(ctx.base, fd, EV_WRITE | EV_PERSIST, on_write, (void*)&ctx);
    if(!ctx.write_event) {
        ERR("Failed event_new() for EV_WRITE");
        return;
    }

    if(event_add(ctx.read_event, NULL) < 0) {
        ERR("Failed event_add()");
        return;
    }
}

static void on_close(ConnCtx& ctx) {
    INF("Closing fd %d", ctx.fd);
    event_del(ctx.read_event);
    event_free(ctx.read_event);

    if(ctx.write_event) {
        event_del(ctx.write_event);
        event_free(ctx.write_event);
    }

    close(ctx.fd);
}

void Server::run(const char* host, uint16_t port) {
    if (lst.size()) {
        WRN("On initialize, other socet already used");
        lst.clear();
    }

    auto first = lst.emplace_front();

    first.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (first.fd < 0) {
        ERR("Failed create socket");
        return;
    }

    if(evutil_make_socket_nonblocking(first.fd) < 0) {
        ERR("Failed call evutil_make_socket_nonblocking()");
        return;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = inet_addr(host);
    if(bind(first.fd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        ERR("Binding failed on %s:%u", host, port);
        return;
    }

    if(listen(first.fd, 1000) < 0) {
        ERR("Listening failed");
        return;
    }

    auto base = event_base_new();
    if(!base) {
        ERR("Failed event_base_new()");
        return;
    }

    auto accept_event = event_new(base, lst.front().fd, EV_READ | EV_PERSIST, on_accept, (void*)&lst);
    if(!accept_event) {
        ERR("Failed event_new()");
        return;
    }

    lst.front().base = base;
    lst.front().read_event = accept_event;

    if(event_add(accept_event, NULL) < 0) {
        ERR("Failed event_add()");
        return;
    }

    if(event_base_dispatch(base) < 0) {
        ERR("Failed event_base_dispatch()");
        return;
    }

    // free 
    on_close(lst.front());
    event_base_free(base);

}


} // namespace ttm
