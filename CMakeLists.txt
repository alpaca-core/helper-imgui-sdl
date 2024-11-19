# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(ac-imgui-sdl
    VERSION 1.0.0
    DESCRIPTION "Alpaca Core CMake wrapper of Dear ImGui and SDL"
    LANGUAGES CXX
)

# cpm
include(./get_cpm.cmake)

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.5.4)

list(APPEND CMAKE_MODULE_PATH
    "${icm_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(icm_dev_mode)
include(icm_add_lib)

#######################################
# options

option(AC_IMGUI_SDL_REQUIRED "${PROJECT_NAME}: require that the library is available" ${ICM_DEV_MODE})
option(AC_IMGUI_SDL_BUILD_EXAMPLES "${PROJECT_NAME}: build examples" ${ICM_DEV_MODE})

option(AC_IMGUI_SDL_ENABLE_DEMO_WINDOWS "${PROJECT_NAME}: enable ImGui demo windows" ${ICM_DEV_MODE})

#######################################
# packages

find_package(SDL2)
if(NOT SDL2_FOUND)
    if(AC_IMGUI_SDL_REQUIRED)
        message(FATAL_ERROR "${PROJECT_NAME}: Could not find SDL")
    else()
        message(STATUS "${CMAKE_PROJECT_NAME}: SDL was not found. ImGui examples will be unavailable")
        return()
    endif()
endif()

CPMAddPackage(gh:iboB/splat@1.3.3) # for SYMBOL_EXPORT
CPMAddPackage(gh:ocornut/imgui@1.90.9)

#######################################
# targets

icm_add_lib(imgui-sdl IMGUI
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)
target_include_directories(imgui-sdl
    PUBLIC
        ${imgui_SOURCE_DIR}
    INTERFACE
        ${imgui_SOURCE_DIR}/backends
        ${imgui_SOURCE_DIR}/misc/cpp
)
target_link_libraries(imgui-sdl
    INTERFACE
        SDL2::SDL2main
    PUBLIC
        splat::splat
        SDL2::SDL2
)

if(AC_IMGUI_SDL_ENABLE_DEMO_WINDOWS)
    set(IMGUI_DISABLE_DEMO_WINDOWS OFF)
else()
    set(IMGUI_DISABLE_DEMO_WINDOWS ON)
endif()

configure_file(imgui_userconfig.in.h imgui_userconfig.h @ONLY)

target_compile_definitions(imgui-sdl PUBLIC IMGUI_USER_CONFIG="${CMAKE_CURRENT_BINARY_DIR}/imgui_userconfig.h")
add_library(ac-dev::imgui-sdl ALIAS imgui-sdl)

add_library(imgui-sdl-app INTERFACE)
add_library(ac-dev::imgui-sdl-app ALIAS imgui-sdl-app)
target_include_directories(imgui-sdl-app INTERFACE app)
target_link_libraries(imgui-sdl-app INTERFACE imgui-sdl)

if(AC_IMGUI_SDL_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
