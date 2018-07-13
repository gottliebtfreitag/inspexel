#pragma once

#include "Parameter.h"

#include <string>
#include <regex>
#include <set>

namespace parameter
{

/**
 * parse the arguments and set all parameters with the parsed values
 */
void parseArguments(int argc, char const* const* argv);
/**
 * parse the arguments but only set the parameters specified in the set to the parsed values
 */
void parseArguments(int argc, char const* const* argv, std::set<ParameterBase*> const& targetParameters);

std::string generateHelpString(std::regex const& filter=std::regex{".*"});

std::set<std::string> getNextArgHint(int argc, char const* const* argv);

void callCommands();

}
