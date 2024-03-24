message(STATUS "CMake: server")

project(server)

set(SOURCES
	server.c
	main.c
)

add_executable(server.exe ${SOURCES})
target_link_libraries(server.exe wsock32 ws2_32)

