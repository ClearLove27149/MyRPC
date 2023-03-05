#include <iostream>
#include <string>
#include <vector>
#define  MSGPACK_NO_BOOST
#include "asio.hpp"
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
    ip::tcp::acceptor acceptor(iosev,ip::tcp::endpoint(ip::tcp::v4(), 8080));

    for(;;) {
        // socket对象
        ip::tcp::socket socket(iosev);
        // 等待直到客户端连接进来
        acceptor.accept(socket);
        //acceptor.async_accept(socktet, [] {std::cout<<"1" <<std::endl;});
        // 显示连接进来的客户端
        std::cout << socket.remote_endpoint().address() << std::endl;
        // 向客户端发送testinfo
        Test_info testInfo = {.id=1, .name="libin", .nums={10086, 10087}};
        // 序列化组包，转成string在网络发送
        std::stringstream sb;
        msgpack::pack(sb, testInfo);
        sb.seekg(0);
        //std::string str(sb.data());
std::cout<<sb.str()<<std::endl;
        asio::error_code  ec;
        socket.write_some(buffer(sb.str()), ec);
        // 如果出错，打印出错信息
        if(ec){
            std::cout <<ec.value() << ec.message() << std::endl;
            break;
        }
        // 与当前客户交互完成后循环继续等待下一客户连接
    }
    return 0;
}