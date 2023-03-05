//
// Created by 26685 on 2023/3/4.
//
#include <iostream>

#include "asio.hpp"

char read_buf[4096];

int main() {
    asio::io_context iosev;
    //asio::ip::tcp::endpoint endpoint{asio::ip::make_address("127.0.0.1"), 80};
    asio::ip::tcp::acceptor acceptor(iosev,asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8080));
    for (;;) {
        asio::ip::tcp::socket socket{iosev};
        acceptor.async_accept(socket, [&](std::error_code err) {
            socket.async_receive(asio::buffer(read_buf), [&](std::error_code err, std::size_t n) {
                socket.async_send(asio::buffer("hello, world\n"), [&](std::error_code err, std::size_t n) {
                    std::cout << read_buf;
                });
            });
        });
        iosev.run();

    }
    //asio::ip::tcp::socket socket{ctx};


}
//
// Created by 26685 on 2023/3/4.
//
