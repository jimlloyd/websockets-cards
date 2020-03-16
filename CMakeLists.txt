cmake_minimum_required (VERSION 3.10.2)

project (socketio-cards)

find_program(CMAKE_C_COMPILER gcc)
find_program(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(uWebSockets)

add_executable(webSocketServer webSocketServer.cpp)
target_link_libraries(webSocketServer uWebSockets)

# TODO: use OPTIONs
target_compile_definitions(webSocketServer PUBLIC LIBUS_NO_SSL)
target_compile_definitions(webSocketServer PUBLIC UWS_NO_ZLIB)
target_compile_definitions(webSocketServer PUBLIC WITH_LIBUV)
