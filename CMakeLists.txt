cmake_minimum_required(VERSION 3.12)
project(OSRenderer)

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
    "upng/upng.h"
    "upng/upng.c"
)



add_executable(OSRenderer ${sources}
"display.h"
"display.cpp"
"renderer.h"
"keyboard.h"
"drawing.h"
"drawing.cpp"
"math.h"
"mesh.h"
"mesh.cpp"
"gui.h"
"mouse.h"
"macros.h"
"shapes.h"
"color.h"
"font.h"
"cimgui.h"
"scene_controller.h"
"light.h"
"light.cpp"
"texture.h"
"texture.cpp"
"triangle.h"
"triangle.cpp" "camera.h" "clipping.h" "clipping.cpp" "polygon.h" "polygon.cpp" "graphic_pipeline.h" "graphic_pipeline.cpp")


target_include_directories(OSRenderer PRIVATE deps/SDL2/include
                                            deps/SDL2_TTF/include        
                                            deps/SDL2_IMAGE/include        
                                            imgui
                                            upng)

target_link_directories(OSRenderer PRIVATE deps/SDL2/lib/x86  
                                         deps/SDL2_TTF/lib
                                         deps/SDL2_IMAGE/lib
                                         )

target_link_libraries(OSRenderer PRIVATE opengl32.lib
                                     SDL2.lib                                     
                                     SDL2main.lib
                                     SDL2_ttf.lib                                     
                                     SDL2_image.lib                                     
)
