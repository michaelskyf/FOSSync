set(SERVER_NAME "${PROJECT_NAME}-server")

add_executable(
	${SERVER_NAME}
	main.c
	args.c
	worker.c
	)

find_package(OpenSSL REQUIRED)

target_link_libraries(
	${SERVER_NAME}
	${OPENSSL_LIBRARIES}
	selog
	)
