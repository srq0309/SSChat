cmake_minimum_required(VERSION 3.4)

project(SSChat-server)

include(${PROJECT_SOURCE_DIR}/../../script/custom_binary.cmake)
if(WIN32)
    include(${PROJECT_SOURCE_DIR}/../../script/custom_msvc.cmake)    
endif(WIN32)

include_directories(${PROJECT_SOURCE_DIR}/../../include)

file(GLOB SERVER_SRC
	../../include/*.h
    ./src/*.h
    ./src/*.cpp
)

add_executable(SSChat-server ${SERVER_SRC})