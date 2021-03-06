find_package(Qt5Widgets REQUIRED)

# generate windows .rc file for this binary
if (WIN32)
	include(${CMAKE_SOURCE_DIR}/cmake/win/win_rc.cmake)
	generate_win_rc_file(hyperhdr)
endif()

add_executable(hyperhdr
	console.h
	hyperhdr.h
	systray.h
	hyperhdr.cpp
	systray.cpp
	main.cpp
	${hyperhdr_WIN_RC_PATH}
)

# promote hyperhdr as GUI app
if (WIN32)
	target_link_options(hyperhdr PUBLIC /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
else()
	set_target_properties(hyperhdr PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

target_link_libraries(hyperhdr
	commandline
	hyperhdr-base
	effectengine
	jsonserver
	flatbufserver
	protoserver
	webserver
	ssdp
	database
	resources
	Qt5::Widgets
)

if (ENABLE_AVAHI)
	target_link_libraries(hyperhdr bonjour)
endif ()

if (ENABLE_V4L2)
	target_link_libraries(hyperhdr v4l2-grabber)
endif ()

if (ENABLE_WMF)
	target_link_libraries(hyperhdr WMF-grabber)
endif ()

if (DEFAULT_SOUNDCAPWINDOWS)
	target_link_libraries(hyperhdr SoundCapWindows)
endif ()

if (DEFAULT_SOUNDCAPLINUX)
	target_link_libraries(hyperhdr SoundCapLinux)
endif ()

if(NOT WIN32)
	install ( TARGETS hyperhdr DESTINATION "share/hyperhdr/bin" COMPONENT "HyperHDR" )
	install ( DIRECTORY ${CMAKE_SOURCE_DIR}/bin/service DESTINATION "share/hyperhdr/" COMPONENT "HyperHDR" )
	install ( FILES ${CMAKE_SOURCE_DIR}/resources/icons/hyperhdr-icon-32px.png DESTINATION "share/hyperhdr/icons" COMPONENT "HyperHDR" )

	# Desktop file for hyperhdr
	install ( FILES ${CMAKE_SOURCE_DIR}/cmake/desktop/hyperhdr_128.png DESTINATION "share/hyperhdr/desktop" COMPONENT "HyperHDR" )
	install ( FILES ${CMAKE_SOURCE_DIR}/cmake/desktop/hyperhdr.desktop DESTINATION "share/hyperhdr/desktop" COMPONENT "HyperHDR" )
else()
	install ( TARGETS hyperhdr DESTINATION "bin" COMPONENT "HyperHDR" )

	#set( CMAKE_INSTALL_UCRT_LIBRARIES TRUE )
	#set( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE )
	include( InstallRequiredSystemLibraries )
endif()

if(CMAKE_HOST_UNIX)
	install( CODE "EXECUTE_PROCESS(COMMAND ln -sf \"../share/hyperhdr/bin/hyperhdr\" \"${CMAKE_BINARY_DIR}/symlink_hyperhdr\" )" COMPONENT "HyperHDR" )
	install( FILES ${CMAKE_BINARY_DIR}/symlink_hyperhdr DESTINATION "bin" RENAME hyperhdr COMPONENT "HyperHDR" )
	install( CODE "FILE (REMOVE ${CMAKE_BINARY_DIR}/symlink_hyperhdr )" COMPONENT "HyperHDR" )
endif()

if(WIN32)
	if(NOT CMAKE_GITHUB_ACTION)
		get_target_property(QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
		get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
		find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
		
		if (WINDEPLOYQT_EXECUTABLE)
			set(WINDEPLOYQT_PARAMS --verbose 0 --no-compiler-runtime --no-opengl-sw --no-system-d3d-compiler)
			add_custom_command(TARGET hyperhdr POST_BUILD COMMAND ${WINDEPLOYQT_EXECUTABLE} ${WINDEPLOYQT_PARAMS} "$<TARGET_FILE:hyperhdr>")
		endif(WINDEPLOYQT_EXECUTABLE)
		
		if (TURBOJPEG_FOUND)
			find_file(TurboJPEG_DLL
				NAMES "turbojpeg.dll" "jpeg62.dll"
				PATHS "${CMAKE_SOURCE_DIR}/resources/dll/jpeg"
				NO_DEFAULT_PATH
			)
			add_custom_command(TARGET hyperhdr POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TurboJPEG_DLL} "$<TARGET_FILE_DIR:hyperhdr>")		
		endif(TURBOJPEG_FOUND)
	endif()
endif()

# Copy dependencies (not for OSX)
include(${CMAKE_SOURCE_DIR}/cmake/Dependencies.cmake)

if (NOT WIN32) # Unix
	DeployUnix("hyperhdr")
elseif(WIN32) # Windows
	DeployWindows("hyperhdr")
endif ()
