#pragma once

#include <string>
#include <regex>
#include <set>

#include "Parameter.h"

namespace sargp
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
std::string generateGroffString();

std::set<std::string> getNextArgHint(int argc, char const* const* argv);

void callCommands();

}
