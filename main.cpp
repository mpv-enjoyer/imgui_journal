#include "render/render.h"

int main(int, char**)
{
    //{ Test_Journal t; }
    Journal journal;
    if (!journal.load()) journal.generate();
    Graphical graphical(journal);
    Render render(&journal, &graphical);
    render.main_loop();
    journal.save();
    return 0;
}