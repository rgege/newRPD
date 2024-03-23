message(STATUS "CMake: server")

project(server)

set (CMAKE_C_FLAGS )


set(SOURCES
	server.c
	main.c
)

add_executable(server.exe ${SOURCES})
target_link_libraries(server.exe wsock32 ws2_32)

