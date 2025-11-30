cmake_minimum_required(VERSION 3.0)

set(PROJECT growbuf)
project(${PROJECT} VERSION 1.0.0 LANGUAGES C)
include(CTest)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

add_compile_options(-std=c99)
if(ENABLE_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Coverage enabled")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else()
        add_compile_options(-O3 -g3)
        message(WARNING "Coverage is not supported for compiler: ${CMAKE_C_COMPILER_ID}")
    endif()
endif()

add_library(${PROJECT} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/growbuf.c)
target_include_directories(${PROJECT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(${PROJECT} PRIVATE -Wall -Wextra)
    
if(BUILD_TESTING)
    enable_testing()
    set(TEST_TARGETS "")
    file(GLOB TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.c")
    foreach(TEST_SRC ${TEST_SRCS})
        get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SRC})
        target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT})
        target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/tests)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
        list(APPEND TEST_TARGETS ${TEST_NAME})
    endforeach()

    if(ENABLE_COVERAGE)
        add_custom_target(coverage
            DEPENDS ${TEST_TARGETS}
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -T Test -T Coverage
        )
    endif()
endif()
