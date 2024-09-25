#include "render/render.h"
#include "platforms/platforms.h"

int main(int, char**)
{
    if (impl::is_application_already_running()) return 1;
    //{ Test_Journal t; }
    Journal journal;
    if (!journal.load()) journal.generate();
    Graphical graphical(journal);
    Render render(&journal, &graphical);
    render.main_loop();
    journal.save();
    return 0;
}
