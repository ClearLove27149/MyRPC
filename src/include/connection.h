//
// Created by 26685 on 2023/3/4.
//

#ifndef MYRPC_CONNECTION_H
#define MYRPC_CONNECTION_H

#include "asio.hpp"

class Connection {
private:
    asio::ip::tcp::endpoint conn_ep;
    asio::ip::tcp::socket conn_socket;
    //asio::io_service conn_io_service;
    bool active;
public:
    Connection() = default;
    Connection(asio::ip::tcp::endpoint ep, asio::ip::tcp::socket);
    bool isActive();

    bool close();
};
#endif //MYRPC_CONNECTION_H
