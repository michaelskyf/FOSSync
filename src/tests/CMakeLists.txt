add_executable(
	"${PROJECT_NAME}-test"
	main.cpp
	)

target_link_libraries(
	"${PROJECT_NAME}-test"
	GTest::GTest
	GTest::gtest_main
	)

gtest_discover_tests("${PROJECT_NAME}-test")

find_program(VALGRIND "valgrind")
if (VALGRIND)
  set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --error-exitcode=1")
  # include Dart to generate the site configuration:
  # https://gitlab.kitware.com/cmake/community/wikis/doc/ctest/Generating-Testing-Files#using-cmake
  include(Dart)
endif ()
