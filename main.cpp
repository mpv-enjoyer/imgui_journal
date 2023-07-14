// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <string>
#include <limits>
#include <vector>
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWvidmode vidmode;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "The journal itself", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/NotoMonoNerdFont-Regular.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    std::vector<std::string> names{"Петухова Таисия Данииловна", "Рыжова Милана Андреевна", "Кузина Александра Сергеевна", "Лебедева Варвара Давидовна", "Куликов Дмитрий Ильич", "Петрова Вера Михайловна", "Панов Кирилл Иванович", "Дубровина Анна Никитична", "Михайлов Владимир Иванович", "Миронова Елизавета Алексеевна", "Пономарев Андрей Артёмович", "Никулина Дарья Степановна", "Иванов Ян Иванович", "Морозова Есения Марковна", "Мухина Ирина Михайловна", "Леонова Владислава Романовна", "Романов Владимир Владимирович", "Смирнов Роман Вадимович", "Кудряшов Иван Лукич", "Гусев Ростислав Давидович"};


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            static int is_here[15][15];

            for (int i = 0; i < 225; i++)
            {
                is_here[i%15][i/15];

            }
    static bool is_calendar_open = false;
    static bool is_working_out_open = false;
    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.

            static float f = 0.0f;
            static int counter = 0;
            static bool use_work_area = true;
            static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
            bool is_opened_modal;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            
            ImGui::Begin("Hello, world!", nullptr, flags);
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar;
	        {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

            ImGui::Text("Выбран день 14 июля 2023");
            ImGui::SameLine();
            if(ImGui::Button("Изменить день") || is_calendar_open)
            {
                is_calendar_open = true;
                ImGui::OpenPopup("Календарь");
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                ImGui::SetNextWindowBgAlpha(1.0F);
                if (ImGui::BeginPopupModal("Календарь", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("<самодельный календарь>");
                    if (ImGui::Button("Ок")) { ImGui::CloseCurrentPopup(); is_calendar_open = false; }        
                }
                ImGui::EndPopup();
            };
            ImGui::SameLine();
            ImGui::Button("Редактировать расписание занятий");
            ImGui::SameLine();
            ImGui::Button("Добавить ученика");
            ImGui::SameLine();
            ImGui::Button("Журнал оплаты");
            ImGui::SameLine();
            ImGui::Button("Справка");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), " = кнопки работают только для вторых групп = ");


            ImGui::BeginChild("Child", ImVec2(0, 0), true, window_flags);
            for (int j = 0; j < 10; j++)
            {
            ImGui::BeginGroup();
            std::string fake_time = "Группа 1, " + std::to_string(j+8)+":00";
            ImGui::Text(fake_time.c_str());
            if (ImGui::BeginTable(
            fake_time.c_str(), 
            4, 
            ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX,
            ImVec2(std::numeric_limits<float>::max(),(0.0F))))

            {
                for (int i = 0; i < 20; i++)
                {
                    char buf[32];
                    ImGui::TableNextColumn();
                    ImGui::Text("# договора?");
                    ImGui::TableNextColumn();
                    ImGui::Text("ФИО ученика %i группы 1      ", i);
                    ImGui::TableNextColumn();
                    ImGui::Text(" ");
                    ImGui::TableNextColumn();
                    //ImGui::Combo("combo", &is_here[i][j], " \0+\0Б\0-\0\0");
                }
                ImGui::EndTable();
            }

            if (j%2!=1)
            {
                int on_lesson = 0;
                for (int i = 0; i < 10; i++)
                {
                    if (is_here[i][j]==1 || is_here[i][j]==4) on_lesson++;
                }
            std::string fake_time2 = "Группа 2, " + std::to_string(j+8)+":00, есть " + std::to_string(on_lesson) + " учеников.";
            ImGui::Text(fake_time2.c_str());
            
            //if (ImGui::BeginTable(fake_time2.c_str(), 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoHostExtendX))
            if (ImGui::BeginTable(fake_time2.c_str(),
            4, 
            ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX,
            ImVec2(std::numeric_limits<float>::max(),(0.0F)))) 
            //numeric_limit max is a bad way to avoid clipping issue
            {
                for (int i = 0; i < 10; i++)
                {
                    ImGui::PushID(i);
                    ImGui::TableNextColumn();
                    ImGui::Text("# договора?");
                    ImGui::TableNextColumn();
                    ImGui::Text(names[(i+j)%20].c_str());
                    ImGui::TableNextColumn();
                    if (is_here[i][j]==0) {ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.4f, 0.4f, 0.4f, 0.8f))); ImGui::Text(" ");} 
                    if (is_here[i][j]==1) {ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.0f, 0.8f, 0.0f, 0.8f))); ImGui::Text("+");}
                    if (is_here[i][j]==2) {ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.0f, 0.8f))); ImGui::Text("Б");}
                    if (is_here[i][j]==3) {ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.8f, 0.0f, 0.0f, 0.8f))); ImGui::Text("-");}
                    if (is_here[i][j]==4) {ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.8f, 0.8f))); ImGui::Text("О");}
                    ImGui::TableNextColumn();
                    if(ImGui::Button("+")) is_here[i][j]=1;
                    ImGui::SameLine();
                    if(ImGui::Button("Б")) is_here[i][j]=2;
                    ImGui::SameLine();
                    if(ImGui::Button("-")) is_here[i][j]=3;  //might need PushID PopID (still idk how they work)
                    ImGui::SameLine();
                    if(ImGui::Button("О")) 
                    {
                        is_here[i][j]=4;
                        is_working_out_open = true;
                    }

                    ImGui::PopID();

                }
                ImGui::EndTable();
            }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            }
            if (is_working_out_open)
                    {
                        ImGui::OpenPopup("Отработка");
                        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                        if (ImGui::BeginPopupModal("Отработка", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                        ImGui::Text("отработка за ФИО ученика ** группы *, **:00", 1,1+8);
                        ImGui::Text("<самодельный календарь, с подсветкой дней, в которые этот человек должен был прийти>");
                        if (ImGui::Button("OK", ImVec2(0, 0))) { ImGui::CloseCurrentPopup(); is_working_out_open=false;}
                        
                        }
                        else
                        {;
                        }
                        ImGui::EndPopup();
                    } 
        }	


		        {
            /*if (disable_mouse_wheel)
                window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
            if (!disable_menu)
                window_flags |= ImGuiWindowFlags_MenuBar;*/
            ImGui::EndChild();
            ImGui::PopStyleVar();
        }

        ImGui::End();        

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}



/*
- Вертикальная прокрутка
- ID | ФИО ученика | № договора | Combobox(тут или нет или отработка) | Combobox(ИЗО/ИЗО+Лепка/Лепка/Спецкурс)
- В каждой таблице должно быть 2 дополнительных строки: для отработчиков и для добавления в группу
*/