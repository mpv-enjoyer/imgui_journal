g++ -std=c++17 -I../imgui -I../imgui/backends -I../imgui/misc/cpp -Wall -Wformat -Wuninitialized -lstdc++fs `pkg-config --cflags glfw3` -O0 -g `sdl2-config --cflags` \
 ../imgui/imgui.cpp ../imgui/imgui_demo.cpp ../imgui/imgui_draw.cpp ../imgui/imgui_tables.cpp ../imgui/imgui_widgets.cpp -I. \
 common/*.cpp controller/*.cpp model/*.cpp view/*.cpp main.cpp