cmake_minimum_required(VERSION 3.16)
project(Game CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

option(SANITIZE "Build project with Address & UB sanitizers" OFF)
option(EMSCRIPTEN "Build project for web" OFF)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/vendor)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# warning level
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -pedantic
        -pedantic-errors
        -Wall
        -Werror
        -Wextra
        -Wno-unused-command-line-argument
        -Wno-unused-parameter
    )
endif()

# sanitizers
if(SANITIZE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("Building with sanitizers")

        target_compile_options(${PROJECT_NAME} PRIVATE
            -fsanitize=address,undefined
            -fno-sanitize-recover=all
        )

        target_link_libraries(${PROJECT_NAME} -fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "Sanitizers are supported only on Clang!")
    endif()
endif()

if(EMSCRIPTEN)
    message("WAVE: Building for web")
    set(PLATFORM Web)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

    add_definitions(-DPLATFORM_WEB)
    add_definitions(-DUSE_WASM)

    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/)

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Os -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy -s GL_ENABLE_GET_PROC_ADDRESS -s USE_GLFW=3 --embed-file assets/ --shell-file ${CMAKE_SOURCE_DIR}/src/index.html")

    string(REPLACE "-rdynamic" "" CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

add_subdirectory(${THIRDPARTY_DIR}/raylib)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_include_directories(${PROJECT_NAME} PUBLIC SYSTEM ${THIRDPARTY_DIR}/raylib/src ${THIRDPARTY_DIR}/HLAM)
target_link_libraries(${PROJECT_NAME} raylib)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
