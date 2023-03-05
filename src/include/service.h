//
// Created by 26685 on 2023/3/4.
//
#pragma once
#ifndef MYRPC_SERVICE_H
#define MYRPC_SERVICE_H
#include "asio.hpp"
#include "connection.h"

#include <iostream>
#include <functional>
#include <string>
#include <unordered_map>
/***
 * service 管理所有的服务，可以是普通函数、lambda表达式等
 *
 */
 template<typename Function, typename... Args>
 class ServiceInstance : asio::noncopyable {
     std::unique_ptr<std::function<Function>> Instance;
     auto Bind(Function f, Args... args);

     bool register_handler(std::string name, Function f);

     bool call();
 };
template<typename Function, typename... Args>
 class Service : asio::noncopyable {
 public:
     //using ServiceInstance = std::unique_ptr<std::function<Function>>;
     // string(ip:port) -> allfunc
     std::unordered_map<std::string, std::vector<ServiceInstance<Function, Args...>>> allService;
     Service() = default;

     bool register_handler(Connection& conn, std::string name, Function f);
 private:
     Service(Service&) = delete;
     Service(Service&&) = delete;

 };


#endif //MYRPC_SERVICE_H
