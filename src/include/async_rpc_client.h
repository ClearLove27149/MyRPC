//
// Created by 26685 on 2023/3/4.
//
#pragma once
#ifndef MYRPC_ASYNC_RPC_CLIENT_H
#define MYRPC_ASYNC_RPC_CLIENT_H

//
// Created by 26685 on 2023/3/4.
//
#include <iostream>
#include <functional>

#define  MSGPACK_NO_BOOST
#include "asio.hpp"
#include "msgpack.hpp"
//#include "util.h"

using namespace asio;

template <typename T>
void print(T t)
{
    std::cout << t << std::endl;
}

template <typename T, typename ...Args>
void print(T t, Args... args)
{
    std::cout << t << std::endl;
    print(args...);
}

class Async_Rpc_Client : asio::noncopyable {
private:
    io_context iosev;
    asio::ip::tcp::endpoint endpoint;
    asio::ip::tcp::socket socket{iosev};
    char read_buf[4096];
    char write_buf[4096];

public:
    Async_Rpc_Client() = default;
    Async_Rpc_Client(std::string ip, uint32_t port) {
        endpoint = {asio::ip::make_address(ip), asio::ip::port_type(port)};
        memset(read_buf, 0 ,4096);
        memset(write_buf, 0 , 4096);
    }
    //typename std::enable_if<!std::is_void<RetType>::value, RetType>::type
    template<typename RetType, typename... Args>
    msgpack::sbuffer _call(const std::string &rpc_name, Args ...args) {
        auto p = std::make_shared<std::promise<RetType>>();
        std::future<RetType> future = p->get_future();

        std::cout<<rpc_name<<std::endl;

        print(args...);
        msgpack::sbuffer sb(4096);
        //msgpack::pack(sb, rpc_name);
        auto args_temp = std::forward_as_tuple(std::forward<Args>(args)...);
        msgpack::pack(sb, std::forward_as_tuple(rpc_name, args_temp));
        return sb;
    }
    void run() {
        iosev.run();
    }
    template<typename RetType, typename... Args>
    void call(const std::string &rpc_name, Args ...args) {
        //for(;;) {
        //asio::error_code ec;
//        socket.connect(endpoint, ec);
//        if(ec) {
//            std::cout << ec.value() << ec.message() << std::endl;
//            return;
//        }
            socket.async_connect(endpoint, [&](std::error_code err) {
                auto sb = _call<RetType>(rpc_name, std::forward<Args>(args)...);
                std::string data(sb.data());
                //std::cout<<sb.data()<<std::endl;
                socket.async_send(buffer(data), [&](std::error_code err, std::size_t n) {
                    socket.async_receive(asio::buffer(read_buf), [&](std::error_code err, std::size_t n) {
                        msgpack::unpacked _msg;
                        msgpack::unpack(_msg, read_buf, n);
                        msgpack::object obj = _msg.get();
                        std::cout << "result: " <<obj.as<RetType>() <<std::endl;
                    });
                });
            });
            iosev.run();
        //}
    }

};

#endif //MYRPC_ASYNC_RPC_CLIENT_H
