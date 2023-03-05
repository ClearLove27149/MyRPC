//
// Created by 26685 on 2023/3/4.
//
#include <iostream>
#include <vector>
#include <string>

#define  MSGPACK_NO_BOOST
#include <asio.hpp>
#include "msgpack.hpp"
//#define  MSGPACK_NO_BOOST
struct Test_info {
    int id;
    std::string name;
    std::vector<int> nums;
    MSGPACK_DEFINE(id, name, nums);
};

int main(int argc, char* argv[]) {
    using namespace asio;
    // 所有asio类都需要io_service对象
    io_service iosev;
    // socket对象
    ip::tcp::socket socket(iosev);
    // 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
    ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 8080);
    // 连接服务器
    asio::error_code ec;
    socket.connect(ep,ec);
    // 如果出错，打印出错信息
    if(ec) {
        std::cout << ec.value() << ec.message() << std::endl;
        return -1;
    }
    // 接收数据
    // 反序列化解包
    //std::string str;
    char buf[100];

    size_t len=socket.read_some(buffer(buf), ec);

    // 反序列化解包
    msgpack::object_handle oh = msgpack::unpack(buf,len);
    msgpack::object obj = oh.get();
    // convert testinfo
    Test_info testInfo;
    obj.convert(testInfo);
    // output
    std::cout << testInfo.id <<std::endl;
    std::cout << testInfo.name <<std::endl;
    for (auto it : testInfo.nums ) {
        std::cout << it <<std::endl;
    }
    //std::cout.write(buf, len);
    std::cout.flush();
    getchar();
    return 0;
}