set -x
IMGUI_DIR="thirdparty/imgui"
INCLUDES="-I$IMGUI_DIR -I$IMGUI_DIR/backends -I$IMGUI_DIR/misc/cpp -I."
CCFLAGS="-std=c++17 -Wall -Wformat -Wuninitialized `pkg-config --cflags glfw3` `sdl2-config --cflags` -g"
LIBS="-lstdc++fs -lGL -licui18n -licuuc -l:imgui.a -Lthirdparty `pkg-config --static --libs glfw3` `sdl2-config --libs`"
CPPFILES="controller/*.cpp model/*.cpp view/platforms/*.cpp main.cpp"
g++ $CCFLAGS $INCLUDES $CPPFILES $LIBS