file(GLOB MSG_PERSISTENT_SRC
    ../../../include/ssimdef.h
    ../../../include/ssim_server_interface.h

    msg_persistent.h
    msg_persistent.cpp

	# sqlite
	../../../../third/sqlite-amalgamation-3250200/*.h
	../../../../third/sqlite-amalgamation-3250200/*.c
)

include_directories(${PROJECT_SOURCE_DIR}/../../../third/sqlite-amalgamation-3250200)

add_definitions(-DSSIM_EXPORTS)
add_library(ssim_msg_persistent SHARED ${MSG_PERSISTENT_SRC})