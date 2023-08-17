cmake_minimum_required(VERSION 3.16)
# YourProjectName 需要修改为你的项目名称 比如我这边叫“ProgramStructure”
project(Myserver)

#使用C++ 14 标准
set(CMAKE_CXX_STANDARD 14)
# 添加要编译的源文件
set(SOURCES
    ../Myserver.cpp
    ../Socket/Socket.cpp
    ../HTTPServer/HTTPServer.cpp
    # 添加其他 .cpp 文件
)
# 添加要编译的头文件
set(HEADERS
    ../Socket/Socket.hpp
    ../HTTPServer/HTTPServer.hpp
)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")
#add_compile_options(-lpthread)
add_executable(Myserver ${SOURCES} ${HEADS})
# 生成可执行文件