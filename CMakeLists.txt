cmake_minimum_required (VERSION 3.0)
project (fell)

set(HEADERS
    src/util/vec.h
)

set(SOURCES
    src/fell.c
    src/util/vec.c
)


include_directories(
    src/util
)

add_executable(fell ${SOURCES} ${HEADERS})
install(TARGETS fell DESTINATION bin)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../man/fell.1 DESTINATION share/man/man1)
