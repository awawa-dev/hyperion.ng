
# Define the current source locations
SET(CURRENT_HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
SET(CURRENT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/libsrc)

add_subdirectory(hyperhdrbase)
add_subdirectory(commandline)
add_subdirectory(blackborder)
add_subdirectory(jsonserver)
add_subdirectory(flatbufserver)
add_subdirectory(protoserver)
add_subdirectory(ssdp)
add_subdirectory(boblightserver)
add_subdirectory(leddevice)
add_subdirectory(utils)
add_subdirectory(effectengine)
add_subdirectory(grabber)
add_subdirectory(webserver)
add_subdirectory(db)
add_subdirectory(api)

if(ENABLE_AVAHI)
	add_subdirectory(bonjour)
endif()

