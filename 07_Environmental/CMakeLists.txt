cmake_minimum_required(VERSION 3.0)

set(PROJECT rhasher)

project(${PROJECT} C)

option(USE_READLINE "Enable linking with readline if available" ON)


find_path(RHASH_INCLUDE_DIR NAMES rhash.h)
find_library(RHASH_LIBRARY NAMES rhash librhash)

if(NOT RHASH_LIBRARY OR NOT RHASH_INCLUDE_DIR)
  message(FATAL_ERROR "Required library 'rhash' not found. Install librhash and headers.")
else()
  message(STATUS "Checking for librhash - found")
endif()

include_directories(${RHASH_INCLUDE_DIR})
set(LIBRARIES ${RHASH_LIBRARY})


if(USE_READLINE)
  find_path(Readline_ROOT_DIR NAMES include/readline/readline.h)
  find_path(Readline_INCLUDE_DIR NAMES readline/readline.h HINTS ${Readline_ROOT_DIR}/include)
  find_library(Readline_LIBRARY NAMES readline HINTS ${Readline_ROOT_DIR}/lib)
  
  if(Readline_INCLUDE_DIR AND Readline_LIBRARY)
    set(readline_FOUND TRUE)
  else()
    set(readline_FOUND FALSE)
  endif()

  if(readline_FOUND)
    include_directories(${Readline_INCLUDE_DIR})
    list(APPEND LIBRARIES ${Readline_LIBRARY})
    add_definitions(-DUSE_READLINE)
    message(STATUS "Checking for libreadline - found")
  else()
    message(STATUS "Checking for libreadline - not found")
  endif()
else()
  message(STATUS "Readline disabled by user (USE_READLINE=OFF)")
endif()

add_executable(${PROJECT} rhasher.c)
target_link_libraries(${PROJECT} ${LIBRARIES})

enable_testing()

# Register a simple test that runs our helper script which compares
# output with system sha1sum/md5sum. The test script accepts the
# built executable path as its first argument.
add_test(NAME check_hashes
  COMMAND ${CMAKE_COMMAND} -E env bash ${CMAKE_SOURCE_DIR}/tests/check_hashes.sh ${CMAKE_BINARY_DIR}/rhasher
)

add_custom_target(distclean
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
  COMMENT "Remove CMake files and generated binaries"
)
set_target_properties(distclean PROPERTIES
  EXCLUDE_FROM_ALL TRUE
)
