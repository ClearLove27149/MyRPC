//
// Created by 26685 on 2023/3/4.
//
#pragma once
#ifndef MYRPC_ASYNC_RPC_SERVER_H
#define MYRPC_ASYNC_RPC_SERVER_H

#include <iostream>
#include <unordered_map>
#include <functional>

#include <asio.hpp>
#define  MSGPACK_NO_BOOST
#include "msgpack.hpp"
#include "util.h"

using namespace asio;


template<typename Function>
class Async_Rpc_Server : asio::noncopyable {
private:
    io_context iosev;
    asio::ip::tcp::endpoint endpoint;
    std::unordered_map<std::string, Function> func_map;

    char read_buf[4096] = {0,};
    char write_buf[4096] = {0,};

    auto call(std::size_t n) {
        msgpack::unpacked _msg;
        msgpack::unpack(_msg, read_buf, n);
        msgpack::object obj = _msg.get();
        // get args,这里应该根据function_traits得到参数和返回值类型

        auto temp = obj.as<std::tuple<std::string>>();
        auto func_name = std::get<0>(temp);
        std::cout<<"Server: "<<func_name<<std::endl;
        for (auto fmap : func_map) {
            if (fmap.first == func_name) {
                int nums_args = function_traits<decltype(fmap.second)>::arity;

                auto all = obj.as<std::tuple<std::string, all_args_t<decltype(fmap.second)>>>();
                auto args = std::get<1>(all);
                auto ret = std::apply(fmap.second, args); // c++17

                msgpack::sbuffer sb(4096);
                msgpack::pack(sb, ret);
                return sb;
                //strncpy(write_buf, ret, func_name.length());
            }
        }
//        msgpack::object_handle oh = msgpack::unpack(read_buf, n);
//        msgpack::object obj = oh.get();
        //std::string func_name = std::get<0>(p);
        //std::cout<<"func_name : "<<func_name<<std::endl;
        //strncpy(write_buf, func_name.c_str(), func_name.length());

    }

public:
    Async_Rpc_Server() = default;
    Async_Rpc_Server(std::string ip, uint32_t port) {
        endpoint = {asio::ip::tcp::v4(), asio::ip::port_type(port)};
    }
    bool register_handler(std::string name, Function f) {
        func_map[name] = std::move(f);
        return true;
    }
    void start() {
        //for (;;) {
            asio::ip::tcp::socket socket{iosev};
            ip::tcp::acceptor acceptor(iosev,ip::tcp::endpoint(ip::tcp::v4(), 8080));
            //acceptor.accept(socket);
            acceptor.async_accept(socket, [&](std::error_code err) {
                //std::cout << "start" << std::endl;
                //std::cout << socket.remote_endpoint().address() << std::endl;

                socket.async_receive(asio::buffer(read_buf, 4096), [&](std::error_code err, std::size_t n) {
                    //std::cout<<n<<std::endl;
                    if (n > 0) {
                        std::cout<<n<<std::endl;
                        auto ret = call(n);
                        socket.async_send(asio::buffer(ret.data(), 4096), [&](std::error_code err, std::size_t n) {
                            std::cout << "Server: " << write_buf << std::endl;
                        });
                    }
                    //call(n);

                });
            });
        //}
        iosev.run();
    }
    void run() {
        iosev.run();
    }

};


#endif //MYRPC_ASYNC_RPC_SERVER_H
