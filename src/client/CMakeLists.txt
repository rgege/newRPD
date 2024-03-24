message(STATUS "CMake: client")

project(client)

set(SOURCES
	client.c
	main.c
)

add_executable(client.exe ${SOURCES})
target_link_libraries(client.exe wsock32 ws2_32)

