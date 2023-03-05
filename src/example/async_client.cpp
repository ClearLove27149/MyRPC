//
// Created by 26685 on 2023/3/4.
//
#include <iostream>

#include "asio.hpp"

char read_buf[4096];

int main() {
    asio::io_context ctx;
    asio::ip::tcp::endpoint endpoint{asio::ip::make_address("127.0.0.1"), 8080};
    asio::ip::tcp::socket socket{ctx};

    socket.async_connect(endpoint, [&](std::error_code err) {
        socket.async_send(asio::buffer("GET / HTTP/1.0\r\n\r\n"), [&](std::error_code err, std::size_t n) {
            socket.async_receive(asio::buffer(read_buf), [&](std::error_code err, std::size_t n) {
                std::cout << read_buf;
            });
        });
    });

    ctx.run();
}
