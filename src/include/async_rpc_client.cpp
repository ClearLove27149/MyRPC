//
// Created by 26685 on 2023/3/5.
//
#include "async_rpc_client.h"

int main() {
    //asio::io_context ctx;
    Async_Rpc_Client asyncRpcClient(std::string("127.0.0.1"), 8080);
    asyncRpcClient.call<int,int,int>(std::string("add"), 1, 2);
    //asyncRpcClient.run();
    //Sleep(5000);
}