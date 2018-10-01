file(GLOB NETWORK_SRC
    ../../../include/ssimdef.h
    ../../../include/ssim_server_interface.h

    network.h
    network.cpp
)

add_library(ssim_network SHARED ${NETWORK_SRC})