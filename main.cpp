#include "render/render.h"

NO_IMPLICIT_CONVERSION_T(int, Month);
NO_IMPLICIT_CONVERSION_T(int, Year);

int main(int, char**)
{
    Month month(4);
    Month month2(3);

    //{ Test_Journal t; }
    Journal journal;
    if (!journal.load()) journal.generate();
    Graphical graphical(journal);
    Render render(&journal, &graphical);
    render.main_loop();
    journal.save();
    return 0;
}