#include "render/render.h"

int main(int, char**)
{
    Journal journal;
    if (!journal.load()) journal.generate_current();
    Graphical graphical(journal);
    Render render(&journal, &graphical);
    render.main_loop();
    journal.save();
    return 0;
}