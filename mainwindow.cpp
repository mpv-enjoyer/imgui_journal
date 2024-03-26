#include "mainwindow.h"

int main(int, char**)
{
    Render::prepare_first_frame();
    Render::main_loop();
    return 0;
}