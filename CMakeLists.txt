cmake_minimum_required(VERSION 3.12)
project(OSRenderer)

set(sources
    "examples/teste_polygon.cpp"
    "deps/imgui/imconfig.h"
    "deps/imgui/imgui.cpp"
    "deps/imgui/imgui.h"
    "deps/imgui/imgui_demo.cpp"
    "deps/imgui/imgui_draw.cpp"
    "deps/imgui/imgui_internal.h"
    "deps/imgui/imgui_widgets.cpp"
    "deps/imgui/imstb_rectpack.h"
    "deps/imgui/imstb_textedit.h"
    "deps/imgui/imstb_truetype.h"
    "deps/imgui/imgui_impl_sdl.cpp"
    "deps/imgui/imgui_impl_sdl.h"
    "deps/imgui/imgui_tables.cpp"
    "deps/imgui/imgui_sdl.cpp"
    "deps/imgui/imgui_sdl.h"
    "deps/upng/upng.h"
    "deps/upng/upng.c"
)



add_executable(OSRenderer ${sources}
"src/display.h"
"src/display.cpp"
"src/renderer.h"
"src/keyboard.h"
"src/drawing.h"
"src/drawing.cpp"
"src/cmath.h"
"src/mesh.h"
"src/mesh.cpp"
"src/gui.h"
"src/mouse.h"
"src/macros.h"
"src/shapes.h"
"src/color.h"
"src/font.h"
"src/cimgui.h"
"src/scene_controller.h"
"src/light.h"
"src/light.cpp"
"src/texture.h"
"src/texture.cpp"
"src/triangle.h"
"src/triangle.cpp"
"src/camera.h"
"src/clipping.h"
"src/clipping.cpp"
"src/polygon.h"
"src/polygon.cpp"
"src/graphic_pipeline.h"
"src/graphic_pipeline.cpp"
)


target_include_directories(OSRenderer PRIVATE deps/SDL2/include
                                            deps/SDL2_TTF/include        
                                            deps/SDL2_IMAGE/include       
                                            src
                                            deps/imgui
                                            deps/upng
)

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
