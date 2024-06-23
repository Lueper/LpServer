#include <iostream>

#include "LpServer.h"

int main() {

    LpServer lpServer;

    try {
        lpServer.Run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    return 0;
}