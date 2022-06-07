set(SERVER_NAME "${PROJECT_NAME}-server")

add_executable(
	${SERVER_NAME}
	commands.c
	msg.c
	worker.c
	main.c
	args.c
	)

target_link_libraries(
	${SERVER_NAME}
	)

if(BUILD_TESTING)
	add_subdirectory(tests)
endif()
