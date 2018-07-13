#include "ExceptionPrinter.h"

namespace
{


std::string print_(std::exception const& e, int level) {
	std::string ret = std::string(level, ' ') + e.what();
	try {
		std::rethrow_if_nested(e);
	} catch(const std::exception& nested) {
		ret += "\n" + print_(nested, level+1);
	} catch(...) {}
	return ret;
}

}

std::string ExceptionPrinter::print(std::exception const& e) {
	return print_(e, 0);
}
