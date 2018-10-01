cmake_minimum_required(VERSION 3.4)

project(SSChat-server)

include(${PROJECT_SOURCE_DIR}/../../script/custom_binary.cmake)
if(WIN32)
    include(${PROJECT_SOURCE_DIR}/../../script/custom_msvc.cmake)    
endif(WIN32)

include_directories(${PROJECT_SOURCE_DIR}/../../include)

# asio
add_definitions(-DASIO_STANDALONE)
include_directories(${PROJECT_SOURCE_DIR}/../../../third/asio/asio/include)

# 网络交互层
add_subdirectory(${PROJECT_SOURCE_DIR}/network)

# 数据分发层
add_subdirectory(${PROJECT_SOURCE_DIR}/msg_route)

# 数据处理层
add_subdirectory(${PROJECT_SOURCE_DIR}/msg_process)

# 数据持久层
add_subdirectory(${PROJECT_SOURCE_DIR}/msg_persistent)

# 主函数
file(GLOB SERVER_SRC
    ./src/main.cpp
    ../../include/ssimdef.h
    ../../include/ssim_server_interface.h
)
add_executable(SSChat-server ${SERVER_SRC})