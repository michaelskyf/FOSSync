add_executable(
	"${PROJECT_NAME}-server"
	main.c
	)

find_package(OpenSSL REQUIRED)

target_link_libraries("${PROJECT_NAME}-server" ${OPENSSL_LIBRARIES})
