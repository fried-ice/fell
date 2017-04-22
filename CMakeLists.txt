cmake_minimum_required (VERSION 3.0)
project (fell)

set(SOURCES
    ../src/fell.c
)

add_executable(fell ${SOURCES})
install(TARGETS fell DESTINATION bin)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../man/fell.1 DESTINATION share/man/man1)
