file(GLOB NETWORK_SRC
    ../../../include/ssimdef.h
    ../../../include/ssim_server_interface.h

    network.h
    network.cpp
    session.h
    session.cpp
)

add_definitions(-DSSIM_EXPORTS)
add_library(ssim_network SHARED ${NETWORK_SRC})