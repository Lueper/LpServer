#include <iostream>

#include "LpServer.h"

int main() {

    LpServer lpServer("127.0.0.1", 5000);

    try {
        lpServer.Run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }

    return 0;
}