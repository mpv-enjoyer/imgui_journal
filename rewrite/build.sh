set -x
g++ -std=c++17 -I../imgui -I../imgui/backends -I../imgui/misc/cpp -Wall -Wformat -Wuninitialized -lstdc++fs `pkg-config --cflags glfw3` -O0 -g `sdl2-config --cflags` \
 ../imgui/imgui.cpp ../imgui/imgui_demo.cpp ../imgui/imgui_draw.cpp ../imgui/imgui_tables.cpp ../imgui/imgui_widgets.cpp -I. ../imgui/misc/cpp/imgui_stdlib.cpp \
 ../imgui/backends/imgui_impl_glfw.cpp ../imgui/backends/imgui_impl_sdlrenderer2.cpp ../imgui/backends/imgui_impl_opengl3.cpp ../imgui/backends/imgui_impl_sdl2.cpp \
 controller/*.cpp model/*.cpp view/platforms/*.cpp view/popups/*.cpp view/subwindows/*.cpp main.cpp \
 -lstdc++fs `pkg-config --cflags glfw3` `sdl2-config --cflags` -lGL -licui18n -licuuc `pkg-config --static --libs glfw3` `sdl2-config --libs`