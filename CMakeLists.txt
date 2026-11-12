cmake_minimum_required(VERSION 3.5)
project(hello_world CXX)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 20)   # broke everything, 17 it is
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Drogon CONFIG REQUIRED)

# the .h files aren't listed here and it still works so I'm not touching it
add_executable(hello_world main.cc)
target_link_libraries(hello_world PRIVATE Drogon::Drogon)

# TODO tests
