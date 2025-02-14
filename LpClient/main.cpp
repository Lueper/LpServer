#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "LpClient.h"
#include "LpClientConsole.h"

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

	// _CrtSetBreakAlloc(4810);
#endif

	lpnet::LpClientConsole console;

	console.LoadFile("config/LpConfig.yaml");

	try {
		console.Run();

		while (true) {
			if (console.ProcessCommand() == false) {

			}
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << "\n";
	}

	return 0;
}