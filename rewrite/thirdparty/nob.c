#define NOB_IMPLEMENTATION
#include "nob.h"

void cmd_append_evaluated(Nob_Cmd* nob_cmd, const char* cmd)
{
    FILE *executed = popen(cmd, "r");
    static char buf[256];
    while (fgets(buf, sizeof(buf), executed) != 0)
    {
        /*...*/
    }
    pclose(executed);

    if (*buf == '\n') return; // empty flags

    Nob_String_Builder sb = { 0 };
    nob_sb_append_cstr(&sb, buf);

    Nob_String_View sv_buf = nob_sv_from_cstr(buf);
    Nob_String_View temp = nob_sv_chop_by_delim(&sv_buf, ' ');
    while (temp.count != 0)
    {
        nob_cmd_append(nob_cmd, nob_temp_sv_to_cstr(temp));
        temp = nob_sv_chop_by_delim(&sv_buf, ' ');
    }
}

void add_libs(Nob_Cmd* cmd)
{
    cmd_append_evaluated(cmd, "sdl2-config --libs");
    cmd_append_evaluated(cmd, "pkg-config --static --libs glfw3");
    nob_cmd_append(cmd, "-lGL", "-licui18n", "-licuuc");
}

void add_ccflags(Nob_Cmd* cmd, bool debug)
{
    nob_cc_flags(cmd);
    cmd_append_evaluated(cmd, "sdl2-config --cflags");
    cmd_append_evaluated(cmd, "pkg-config --cflags glfw3");
    if (debug)
    {
        nob_cmd_append(cmd, "-O0", "-g");
    }
    else
    {
        nob_cmd_append(cmd, "-O3");
    }
}

#define IMGUI_DIR "imgui"
void add_includes(Nob_Cmd* cmd)
{
    nob_cmd_append(cmd, "-I" IMGUI_DIR);
    nob_cmd_append(cmd, "-I" IMGUI_DIR "/backends");
    nob_cmd_append(cmd, "-I" IMGUI_DIR "/misc/cpp");
}

void build_object(Nob_Cmd* link_cmd, const char* file_path)
{
    Nob_String_Builder b = { 0 };
    nob_sb_append_cstr(&b, nob_path_name(file_path));
    nob_sb_append_cstr(&b, ".o");
    nob_sb_append_null(&b);

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "g++");
    //add_libs(&cmd);
    add_includes(&cmd);
    add_ccflags(&cmd, true);

    nob_cmd_append(&cmd, "-c");
    nob_cc_inputs(&cmd, file_path);
    const char* output = nob_temp_sv_to_cstr(nob_sb_to_sv(b));
    nob_cc_output(&cmd, output);
    nob_cc_inputs(link_cmd, output);
    if (!nob_cmd_run(&cmd)) abort();
}

void build_objects(Nob_Cmd* link_cmd)
{
    build_object(link_cmd, IMGUI_DIR "/imgui.cpp");
    build_object(link_cmd, IMGUI_DIR "/imgui_demo.cpp");
    build_object(link_cmd, IMGUI_DIR "/imgui_draw.cpp");
    build_object(link_cmd, IMGUI_DIR "/imgui_tables.cpp");
    build_object(link_cmd, IMGUI_DIR "/imgui_widgets.cpp"); 
    build_object(link_cmd, IMGUI_DIR "/misc/cpp/imgui_stdlib.cpp");
    build_object(link_cmd, IMGUI_DIR "/backends/imgui_impl_glfw.cpp");
    build_object(link_cmd, IMGUI_DIR "/backends/imgui_impl_opengl3.cpp");
    build_object(link_cmd, IMGUI_DIR "/backends/imgui_impl_sdlrenderer2.cpp");
    build_object(link_cmd, IMGUI_DIR "/backends/imgui_impl_sdl2.cpp");
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd link_cmd = { 0 };
    nob_cmd_append(&link_cmd, "ar", "-rcs");
    nob_cc_output(&link_cmd, "imgui.a");
    build_objects(&link_cmd);
    if (!nob_cmd_run(&link_cmd)) abort();
    return 0;
}
