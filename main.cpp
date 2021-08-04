
#include <filesystem>
#include "language_main.hpp"


void displayUsage();

int main(int argc, char** argv)
{
	language_main m;
	if (argc > 1) {
		if (_stricmp(argv[1], "-h") == 0) {
			displayUsage();
		}
		else {
			std::string szRunFile = argv[1];
			std::vector<std::string> cl_args;
			for (int i{ 0 }; i < argc; i++) {
				cl_args.push_back(argv[i]);
			}
			return m.run_file(std::filesystem::absolute(szRunFile).string(), cl_args);
		}
	}
	else {
		displayUsage();
		m.repl();
	}
	return 0;
}

void displayUsage() {
	std::cout << "core v" << DEV_FLAG_VERSION << std::endl;
	std::cout << "usage: core *(<option>)" << std::endl;
	std::cout << "\toption      | desc " << std::endl;
	std::cout << "\t -h         | display help message" << std::endl;
	std::cout << "\t <filename> | execute file with remaining args passed to program entry point" << std::endl;
}