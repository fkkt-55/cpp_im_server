cmake_minimum_required(VERSION 3.26)

project(cpp_im_server)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/src/
        ${CMAKE_SOURCE_DIR}/include/
)

add_executable(cpp_im_server main.cpp
        src/server.cpp
        src/server.h
        src/logger.cpp
        src/logger.h
)

target_link_libraries(cpp_im_server
        ${CMAKE_SOURCE_DIR}/lib/win/WS2_32.Lib
)