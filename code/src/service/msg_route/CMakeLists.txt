file(GLOB MSG_ROUTE_SRC
    ../../../include/ssimdef.h
    ../../../include/ssim_server_interface.h

    ts_queue.hpp
    msg_route.h
    msg_route.cpp
)

add_definitions(-DSSIM_EXPORTS)
add_library(ssim_msg_route SHARED ${MSG_ROUTE_SRC})