cmake_minimum_required(VERSION 3.0)

set(PROJECT guesser)
project(${PROJECT} C)

find_package(Gettext REQUIRED)

set(TEXTDOMAIN guesser)
set(langs ru_RU)
set(LOCALE_DIR ${CMAKE_BINARY_DIR}/locale)

make_directory(${LOCALE_DIR})
foreach(lang IN LISTS langs)
    set(pofile ${CMAKE_SOURCE_DIR}/lang/${lang}.po)
    make_directory(${LOCALE_DIR}/${lang})
    make_directory(${LOCALE_DIR}/${lang}/LC_MESSAGES)
    add_custom_command(
        OUTPUT ${LOCALE_DIR}/${lang}/LC_MESSAGES/${TEXTDOMAIN}.mo
        DEPENDS ${pofile}
        COMMAND msgfmt -o
            ${LOCALE_DIR}/${lang}/LC_MESSAGES/${TEXTDOMAIN}.mo
            ${pofile}
    )
    add_custom_target(gen_${lang} ALL DEPENDS
        ${LOCALE_DIR}/${lang}/LC_MESSAGES/${TEXTDOMAIN}.mo
    )
endforeach()

add_executable(${PROJECT} src/guesser.c)
target_compile_definitions(${PROJECT} PRIVATE LOCALEDIR="${LOCALE_DIR}")

add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Remove CMake files and generated binaries"
)

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

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_OUT ${DOXYGEN_DIR}/Doxyfile)

    make_directory(${DOXYGEN_DIR})
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
        ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${DOXYGEN_DIR}
        COMMENT "Generating documentation with Doxygen"
    )
else()
    message(WARNING "Doxygen not found. Skipping documentation generation.")
endif()

