# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_executable(example-ac-imgui
    example.cpp
)
target_link_libraries(example-ac-imgui
    PRIVATE
        ac-dev::imgui-sdl-app
)
