set -x
IMGUI_DIR="thirdparty/imgui"
INCLUDES="-I$IMGUI_DIR -I$IMGUI_DIR/backends -I$IMGUI_DIR/misc/cpp -I."
CCFLAGS="-std=c++17 -Wall -Wformat -Wuninitialized `pkg-config --cflags glfw3` `sdl2-config --cflags`"
LIBS="-lstdc++fs -lGL -licui18n -licuuc -l:imgui.a -Lthirdparty `pkg-config --static --libs glfw3` `sdl2-config --libs`"
g++ $CCFLAGS $INCLUDES controller/*.cpp model/*.cpp view/platforms/*.cpp view/popups/*.cpp view/subwindows/*.cpp main.cpp $LIBS
#g++ -std=c++17 -I$IMGUI_DIR -I$IMGUI_DIR/backends -I$IMGUI_DIR/misc/cpp -Wall -Wformat -Wuninitialized -lstdc++fs -O0 -g `sdl2-config --cflags` \
# $IMGUI_DIR/imgui.cpp $IMGUI_DIR/imgui_demo.cpp $IMGUI_DIR/imgui_draw.cpp $IMGUI_DIR/imgui_tables.cpp $IMGUI_DIR/imgui_widgets.cpp -I. $IMGUI_DIR/misc/cpp/imgui_stdlib.cpp \
# $IMGUI_DIR/backends/imgui_impl_glfw.cpp $IMGUI_DIR/backends/imgui_impl_sdlrenderer2.cpp $IMGUI_DIR/backends/imgui_impl_opengl3.cpp $IMGUI_DIR/backends/imgui_impl_sdl2.cpp \
# controller/*.cpp model/*.cpp view/platforms/*.cpp view/popups/*.cpp view/subwindows/*.cpp main.cpp \
# -lstdc++fs `pkg-config --cflags glfw3` `sdl2-config --cflags` -lGL -licui18n -licuuc `pkg-config --static --libs glfw3` `sdl2-config --libs`

#g++ -std=c++17  -Wall -Wformat -Wuninitialized -lstdc++fs -O0 -g `sdl2-config --cflags` \
# controller/*.cpp model/*.cpp view/platforms/*.cpp view/popups/*.cpp view/subwindows/*.cpp main.cpp \
# -lstdc++fs -l:imgui.a -Lthirdparty `pkg-config --cflags glfw3` `sdl2-config --cflags` -lGL -licui18n -licuuc `pkg-config --static --libs glfw3` `sdl2-config --libs`