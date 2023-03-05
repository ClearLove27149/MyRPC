#include "async_rpc_server.h"
#include <time.h>
int add(int a, int b) {
    return a+b;
}
int main() {
    std::function<int(int, int)> func = add;
    Async_Rpc_Server<std::function<int(int, int)>> asyncRpcServer(std::string("127.0.0.1"), 8080);

    asyncRpcServer.register_handler("add", func);
    asyncRpcServer.start();
    //asyncRpcServer.run();
    //Sleep(10000);
}
