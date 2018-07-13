#include "parameter/Parameter.h"
#include "parameter/ArgumentParsing.h"
#include <iostream>

namespace {

parameter::Parameter<std::optional<std::string>> printHelp{{}, "help", "print this help add a string which will be used in a grep-like search through the parameters"};

}

int main(int argc, char** argv)
{
	if (std::string(argv[argc-1]) == "--bash_completion") {
		auto hints = parameter::getNextArgHint(argc-2, argv+1);
		for (auto const& hint : hints) {
			std::cout << hint << " ";
		}
		return 0;
	}
	// pass everything except the name of the application
	parameter::parseArguments(argc-1, argv+1);

	if (printHelp.isSpecified()) {
		std::cout << parameter::generateHelpString(std::regex{".*" + printHelp.get().value_or("") + ".*"}) << std::endl;
		return 0;
	}
	parameter::callCommands();
	return 0;
}
