#if defined(WIN32) && (!defined(SMOOTH_COMIC_WINDOWS_CONSOLE))
#define SMOOTH_COMIC_ENTRY_WINDOWS_GUI
#endif

#if defined(SMOOTH_COMIC_ENTRY_WINDOWS_GUI)
#include <Windows.h>
#endif


int entry(int argc, char *argv[]);


#ifdef SMOOTH_COMIC_ENTRY_WINDOWS_GUI
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return entry(__argc, __argv);
}
#else
int main(int argc, char *argv[])
{
    return entry(argc, argv);
}
#endif
