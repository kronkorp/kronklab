cmake_minimum_required(VERSION 3.5)
project(kronklab LANGUAGES C)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier build/.")
endif()

file(GLOB_RECURSE kronklab_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(kronklab-obj ${kronklab_SOURCES})

target_include_directories(kronklab-obj
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(kronklab-obj PRIVATE /W3 /O2)
else()
    target_compile_options(kronklab-obj PRIVATE -Wall -Wextra -O3)
endif()

add_library(kronklab-static STATIC
    $<TARGET_OBJECTS:kronklab-obj>
)

target_link_libraries(kronklab-static
    PUBLIC
        kronklab-obj
)

add_library(kronklab-shared SHARED
    $<TARGET_OBJECTS:kronklab-obj>
)

target_link_libraries(kronklab-shared
    PUBLIC
        kronklab-obj
)

add_library(kronklab ALIAS kronklab-static)

set_target_properties(kronklab-static PROPERTIES
    OUTPUT_NAME kronklab
)

set_target_properties(kronklab-shared PROPERTIES
    OUTPUT_NAME kronklab
)

set_target_properties(kronklab-shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_target_properties(kronklab-obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

include(GNUInstallDirs)

install(TARGETS kronklab-static kronklab-shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronklab
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# The sample suite has 1 failing and 1 crashing test on purpose: check the report, not the exit code.
enable_testing()
add_executable(kronklab-tests tests/test.c)
target_link_libraries(kronklab-tests PRIVATE kronklab-static)
add_test(NAME kronklab-tests COMMAND kronklab-tests)
set_tests_properties(kronklab-tests PROPERTIES
    PASS_REGULAR_EXPRESSION "total \\[5\\] \\| passed \\[[^]]*3")
