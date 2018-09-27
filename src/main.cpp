#include "parameter/Parameter.h"
#include "parameter/ArgumentParsing.h"
#include <iostream>

namespace {

auto printHelp = parameter::Parameter<std::optional<std::string>>{{}, "help", "print this help add a string which will be used in a grep-like search through the parameters"};

}

#define TERM_RED                        "\033[31m"
#define TERM_RESET                      "\033[0m"

int main(int argc, char** argv)
{
	if (std::string(argv[argc-1]) == "--bash_completion") {
		auto hints = parameter::getNextArgHint(argc-2, argv+1);
		for (auto const& hint : hints) {
			std::cout << hint << " ";
		}
		return 0;
	}
	try {
		// pass everything except the name of the application
		parameter::parseArguments(argc-1, argv+1);

		if (printHelp.isSpecified()) {
			std::cout << "inspexel version 1.1.0\n";
			std::cout << parameter::generateHelpString(std::regex{".*" + printHelp.get().value_or("") + ".*"});
			return 0;
		}

		parameter::callCommands();
	} catch (std::exception const& e) {
		std::cerr << "exception: " << TERM_RED << e.what() << TERM_RESET "\n";
	}
	return 0;
}
