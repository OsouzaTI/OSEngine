cmake_minimum_required(VERSION 3.12)
project(OSEngine)

set(Project "main.cpp")

add_executable(OSEngine ${Project} "display.h" "display.cpp" "os_engine.hpp" "keyboard.h" "drawing.h" "drawing.cpp" "osmath.hpp"  "mesh.h" "mesh.cpp" "osgui.hpp"  "mouse.h")

target_include_directories(OSEngine PRIVATE deps/SDL2/include)
target_link_directories(OSEngine PRIVATE deps/SDL2/lib/x64)
target_link_libraries(OSEngine PRIVATE SDL2.lib                                     
                                     SDL2main.lib
)
