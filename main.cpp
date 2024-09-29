#include "render/render.h"
#include "platforms/platforms.h"

int main(int argc, char** argv)
{
    if (impl::is_application_already_running()) return 1;
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--enable-legacy-render-driver")
        {
            impl::modern_platform_failed();
            break;
        }
    }
    Journal journal;
    if (!journal.load()) journal.generate();
    Graphical graphical(journal);
    Render render(&journal, &graphical);
    render.main_loop();
    journal.save();
    return 0;
}
