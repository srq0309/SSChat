file(GLOB MSG_PERSISTENT_SRC
    ../../../include/ssimdef.h
    ../../../include/ssim_server_interface.h

    msg_persistent.h
    msg_persistent.cpp
)

add_library(ssim_msg_persistent SHARED ${MSG_PERSISTENT_SRC})