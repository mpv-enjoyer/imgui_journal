#include "render/render.h"
#include "platforms/platforms.h"

int main(int argc, char** argv)
{
    if (Impl::platform()->is_application_already_running()) return 1;
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--enable-legacy-render-driver")
        {
            Impl::prefer_renderer(Impl::Renderers::SDL2);
        }
        if (arg == "--allow-recalculate-all")
        {
            show_button_to_recalculate_all_prices();
        }
        if (arg == "--allow-workout-debugging")
        {
            show_button_to_open_workout_debugging();
        }
        if (arg == "--log-regular-saves")
        {
            turn_on_log_regular_saves();
        }
    }
    Journal journal;
    JournalHolder graphical(journal);
    Render render(&journal, &graphical);
    render.main_loop();
    journal.save();
    return 0;
}
