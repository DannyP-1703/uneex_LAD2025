cmake_minimum_required(VERSION 3.15)

set(PROJECT guesser)
project(${PROJECT} C)

include(GNUInstallDirs)
include(CTest)

## LANGS
find_package(Gettext REQUIRED)

set(TEXTDOMAIN guesser)
set(INSTALL_LOCALE_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALEDIR})
set(langs ru_RU)

foreach(lang IN LISTS langs)
    gettext_process_po_files(
        ${lang} ALL
        PO_FILES ${CMAKE_SOURCE_DIR}/lang/${lang}.po
    )
endforeach()

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT}.pot
    COMMAND xgettext -k_ --package-name ${PROJECT} -o ${CMAKE_BINARY_DIR}/${PROJECT}.pot -D ${CMAKE_SOURCE_DIR} src/guesser.c
    DEPENDS ${CMAKE_SOURCE_DIR}/src/guesser.c
    COMMENT "Generate POT file for translations"
)
    
set(_init_targets)
set(_update_targets)
foreach(lang IN LISTS langs)
    set(pofile ${CMAKE_SOURCE_DIR}/lang/${lang}.po)

    add_custom_target(init-${lang}
        DEPENDS ${CMAKE_BINARY_DIR}/${PROJECT}.pot
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/lang
        COMMAND msginit -i ${CMAKE_BINARY_DIR}/${PROJECT}.pot -l ${lang}.UTF-8 -o ${pofile}
        COMMENT "Init translation for ${lang}"
    )
    list(APPEND _init_targets init-${lang})

    add_custom_target(update-${lang}
        DEPENDS ${CMAKE_BINARY_DIR}/${PROJECT}.pot ${pofile}
        COMMAND msgmerge -U ${pofile} ${CMAKE_BINARY_DIR}/${PROJECT}.pot
        COMMENT "Update translation for ${lang}"
    )
    list(APPEND _update_targets update-${lang})
endforeach()
add_custom_target(init-langs DEPENDS ${_init_targets})
add_custom_target(update-langs DEPENDS ${_update_targets})

## EXEC AND LIB TARGETS

set(LIBRARY roman_nums)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(${LIBRARY} SHARED ${CMAKE_SOURCE_DIR}/src/roman_nums.c)

set(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
add_executable(${PROJECT} ${CMAKE_SOURCE_DIR}/src/guesser.c)
target_link_libraries(${PROJECT} ${LIBRARY})
target_compile_definitions(${PROJECT} PRIVATE LOCALEDIR="${INSTALL_LOCALE_DIR}" TEXTDOMAIN="${TEXTDOMAIN}")
set_target_properties(${PROJECT} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Remove CMake files and generated binaries"
)

## TESTING

if(BUILD_TESTING)
    enable_testing()
    set(TEST_TARGETS "")
    file(GLOB TEST_SRCS "${CMAKE_SOURCE_DIR}/tests/test_*.c")
    foreach(TEST_SRC ${TEST_SRCS})
        get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SRC})
        target_link_libraries(${TEST_NAME} PRIVATE ${LIBRARY})
        target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/tests)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
        list(APPEND TEST_TARGETS ${TEST_NAME})
    endforeach()
endif()


## DOCS

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_DIR ${CMAKE_BINARY_DIR}/doc)
    set(DOXYGEN_OUT ${DOXYGEN_DIR}/Doxyfile)

    make_directory(${DOXYGEN_DIR})
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
        ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${DOXYGEN_DIR}
        COMMENT "Generate documentation with Doxygen"
    )
else()
    message(WARNING "Doxygen not found. Skipping documentation generation.")
endif()


## INSTALL

install(TARGETS ${PROJECT}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS ${LIBRARY}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

foreach(lang IN LISTS langs)
    install(FILES ${CMAKE_BINARY_DIR}/${lang}.gmo
        DESTINATION ${CMAKE_INSTALL_LOCALEDIR}/${lang}/LC_MESSAGES
        RENAME ${TEXTDOMAIN}.mo
    )
endforeach()

install(FILES ${DOXYGEN_DIR}/man/man1/guesser.1
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
    OPTIONAL
)

install(DIRECTORY ${DOXYGEN_DIR}/html
    DESTINATION ${CMAKE_INSTALL_DOCDIR}/html
    OPTIONAL
)


if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif()


