cmake_minimum_required(VERSION 3.10)
project(JeuKenneth)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/Users/super/vcpkg/installed/x64-windows/include")
link_directories("C:/Users/super/vcpkg/installed/x64-windows/lib")

add_executable(server main.cpp)
add_executable(client client.cpp)

target_link_libraries(server PRIVATE enet ws2_32 winmm)
target_link_libraries(client PRIVATE enet ws2_32 winmm)