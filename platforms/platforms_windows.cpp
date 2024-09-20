#include "platforms.h"
#include "windows.h"
#include <iostream>

// Windows: check active
bool is_application_already_running()
{
	HWND find = FindWindowW(NULL, LWINDOW_NAME);
	if (find)
	{
		std::cout << SetActiveWindow(find);
		std::cout << SetFocus(find);
		std::cout << (int)SetForegroundWindow(find); // Doesn't bring window to front but it's icon starts blinking
		std::cout << (int)ShowWindow(find, SW_SHOW);
		std::cout << (int)BringWindowToTop(find);
		return true;
	}
	DWORD current_process_id = GetCurrentProcessId();
	if (!AllowSetForegroundWindow(current_process_id))
	{
		std::cout << GetLastError();
	}
	return false;
    //CreateMutexA(0, FALSE, "Local\\AttendanceJournal101"); // try to create a named mutex
    //if(GetLastError() == ERROR_ALREADY_EXISTS) // did the mutex already exist?
    //{
    //    int msgboxID = MessageBoxW(
    //    NULL,
    //    L"Перейдите в уже открытое окно журнала посещаемости.",
    //    L"Журнал уже открыт",
    //    MB_ICONWARNING);
    //    return true;
    //}    
    //return false;
}

//void set_window_titlebar_icon(GLFWwindow* window)
//{
//	HWND hwnd = glfwGetWin32Window(window); 	
//	HICON hIcon = (HICON) LoadImage(GetModuleHandle(NULL), "icon32", IMAGE_ICON, 32, 32, 0);
//	if (hIcon)
//	{
//		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
//	}
//	else {std::cout << "icon not found";}
//}
