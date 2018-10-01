file(GLOB MSG_PROCESS_SRC
    ../../../include/ssimdef.h
    ../../../include/ssim_server_interface.h

    msg_process.h
    msg_process.cpp
)

add_library(ssim_msg_process SHARED ${MSG_PROCESS_SRC})