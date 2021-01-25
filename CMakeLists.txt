cmake_minimum_required(VERSION 3.12)
project(OSEngine)

set(sources
    main.cpp
    "imgui/imconfig.h"
    "imgui/imgui.cpp"
    "imgui/imgui.h"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_internal.h"
    "imgui/imgui_widgets.cpp"
    "imgui/imstb_rectpack.h"
    "imgui/imstb_textedit.h"
    "imgui/imstb_truetype.h"
    "imgui/imgui_impl_sdl.cpp"
    "imgui/imgui_impl_sdl.h"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_sdl.cpp"
    "imgui/imgui_sdl.h"
)



add_executable(OSEngine ${sources} "display.h"
"display.cpp"
"os_engine.h"
"keyboard.h"
"drawing.h"
"drawing.cpp"
"osmath.h"
"mesh.h"
"mesh.cpp"
"osgui.h"
"mouse.h"
"osmacros.h"
"shapes.h"
"oscolor.h"
"osfont.h"
"osimgui.h"
"oscene_controller.h")


target_include_directories(OSEngine PRIVATE deps/SDL2/include
                                            deps/SDL2_TTF/include        
                                            deps/GLEW/include
                                            imgui)

target_link_directories(OSEngine PRIVATE deps/SDL2/lib/x86  
                                         deps/GLEW/lib
                                         deps/SDL2_TTF/lib)

target_link_libraries(OSEngine PRIVATE opengl32.lib
                                     SDL2.lib                                     
                                     SDL2main.lib
                                     SDL2_ttf.lib                                     
                                     glew32s.lib                                     
)
