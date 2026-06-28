#include "view/view.h"
#include "thirdparty/argh.h"

int main(int argc, char** argv)
{
    argh::parser cmdl(argc, argv);
    for (auto& param : cmdl.params())
    {
        if (param.first == "prefer")
        {
            Impl::prefer_renderer(param.second.c_str());
        }
    }
    View::View().main_loop();
}