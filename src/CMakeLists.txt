set(SERVER_NAME "${PROJECT_NAME}-server")

add_library(
	${SERVER_NAME}-lib
	STATIC
	commands.c
	msg.c
	worker.c
	)

add_executable(
	${SERVER_NAME}
	main.c
	)

target_link_libraries(
	${SERVER_NAME}
	${SERVER_NAME}-lib
	)
