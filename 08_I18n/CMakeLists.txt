cmake_minimum_required(VERSION 3.0)

set(PROJECT guesser)
project(${PROJECT} C)

find_package(Gettext REQUIRED)

set(TEXTDOMAIN guesser)
set(langs ru_RU)

make_directory(${CMAKE_BINARY_DIR}/locale/)
foreach(lang IN LISTS langs)
    set(pofile ${CMAKE_SOURCE_DIR}/lang/${lang}.po)
    make_directory(${CMAKE_BINARY_DIR}/locale/${lang})
    make_directory(${CMAKE_BINARY_DIR}/locale/${lang}/LC_MESSAGES)
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/locale/${lang}/LC_MESSAGES/${TEXTDOMAIN}.mo
        DEPENDS ${pofile}
        COMMAND msgfmt -o
            ${CMAKE_BINARY_DIR}/locale/${lang}/LC_MESSAGES/${TEXTDOMAIN}.mo
            ${pofile}
    )
    add_custom_target(gen_${lang} ALL DEPENDS
        ${CMAKE_BINARY_DIR}/locale/${lang}/LC_MESSAGES/${TEXTDOMAIN}.mo
    )
endforeach()

add_executable(${PROJECT} guesser.c)

add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Remove CMake files and generated binaries"
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT}.pot
    COMMAND xgettext -k_ -o ${CMAKE_BINARY_DIR}/${PROJECT}.pot ${CMAKE_SOURCE_DIR}/guesser.c
    DEPENDS ${CMAKE_SOURCE_DIR}/guesser.c
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
