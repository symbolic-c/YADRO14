#include <iostream>

#include "core/Server.h"

#ifdef _WIN32
    #include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    Server server(25565);
    server.start();
    return 0;
}
