#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>

#include "LpServer.h"

#ifdef _DEBUG
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

int main() {

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

	//_CrtSetBreakAlloc(2807);
#endif

    LpServer* lpServer = new LpServer("192.168.20.241", 7777);

    // Config
    lpServer->LoadFile("config/LpConfig.yaml");

	lpServer->Init();

    try {
        lpServer->Start();
		while (true) {
			if (lpServer->ProcessCommand() == false) {
				lpServer->Stop();
				lpServer->Release();
				break;
			}
		}
    }
    catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }

	delete lpServer;

    return 0;
}