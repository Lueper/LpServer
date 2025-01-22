
#include "LpClient.h"
#include "LpClientConsole.h"

int main() {
	lpnet::LpClientConsole console;

	console.LoadFile("config/LpConfig.yaml");

	console.InitCommand();

	try {
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