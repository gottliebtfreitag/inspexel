#pragma once

#include <string>
#include <stdexcept>

struct ExceptionPrinter {
	static std::string print(std::exception const& e);
};
