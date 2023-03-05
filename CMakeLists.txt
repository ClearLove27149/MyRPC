cmake_minimum_required(VERSION 3.24)
project(MyRPC)

set(CMAKE_CXX_STANDARD 17)


include_directories(
        "src/include/"
        "src/msgpack-c/include/"
        "src/asio/")

add_executable(server src/example/server.cpp)
add_executable(client src/example/client.cpp)

add_executable(async_client src/example/async_client.cpp)
add_executable(async_server src/example/async_server.cpp)

add_executable(async_rpc_client src/include/async_rpc_client.cpp)
add_executable(async_rpc_server src/include/async_rpc_server.cpp)
add_executable(main src/main.cpp)

if(MINGW)
    target_link_libraries(server ws2_32)
    target_link_libraries(client ws2_32)

    target_link_libraries(async_server ws2_32 mswsock)
    target_link_libraries(async_client ws2_32 mswsock)

    target_link_libraries(async_rpc_server ws2_32 mswsock)
    target_link_libraries(async_rpc_client ws2_32 mswsock)

    #target_link_libraries(main ws2_32 mswock)
endif()
