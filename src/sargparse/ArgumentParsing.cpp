#include "ArgumentParsing.h"

namespace sargp
{

namespace
{

bool isArgName(std::string const& str) {
	return str.find("--", 0) == 0;
}

template<typename CommandCallback, typename ParamCallback>
void tokenize(int argc, char const* const* argv, CommandCallback&& commandCB, ParamCallback&& paramCB) {
	int idx{0};
	for (; idx < argc; ++idx) {
		std::string curArgName = std::string{argv[idx]};
		if (isArgName(curArgName)) {
			break;
		} else {
			commandCB(curArgName);
		}
	}
	while (idx < argc) {
		// idx is the index of the last argName that was encountered
		std::string argName = std::string{argv[idx]+2}; // +2 to remove the double dash
		std::vector<std::string> arguments;
		++idx;
		for (; idx < argc; ++idx) {
			std::string v{argv[idx]};
			if (isArgName(v)) {
				break;
			}
			arguments.emplace_back(std::move(v));
		}
		paramCB(argName, arguments);
	}
}

}

void parseArguments(int argc, char const* const* argv) {
	auto const& commands = Command::Registry::getInstance().getCommands();
	std::vector<Command*> argProviders;
	auto range = commands.equal_range("");
	for (auto command = range.first; command != range.second; ++command) {
		argProviders.emplace_back(command->second);
	}
	tokenize(argc, argv, [&](std::string const& commandName){
		auto target = detail::CommandRegistry::getInstance().getCommands().equal_range(commandName);
		if (target.first == target.second) {
			throw std::invalid_argument("command " + commandName + " is not implemented");
		}
		for (auto t{target.first}; t != target.second; ++t) {
			t->second->setActive(true);
			argProviders.push_back(t->second);
		}
	}, [&](std::string const& argName, std::vector<std::string> const& arguments) {
		bool found = false;
		for (auto argProvider : argProviders) {
			auto target = argProvider->getParameters().equal_range(argName);
			if (target.first == target.second) {
				continue;
			}
			found = true;
			for (auto t{target.first}; t != target.second; ++t) {
				try {
					t->second->parse(arguments);
				} catch (sargp::parsing::detail::ParseError const& error) {
					throw std::invalid_argument("cannot parse arguments for \"" + argName + "\" - " + error.what());
				}
			}
		}
		if (not found) {
			throw std::invalid_argument("argument " + argName + " is not implemented");
		}
	});

	// if no commands are active activate all default commands
	bool anyCommandActive = std::find_if(commands.begin(), commands.end(), [](std::pair<std::string, Command*> const& c) {return *c.second;}) != commands.end();
	if (not anyCommandActive) {
		std::for_each(commands.begin(), commands.end(), [](std::pair<std::string, Command*> const& c) {
			if (c.first == "") {
				c.second->setActive(true);
			}
		});
	}
}

void parseArguments(int argc, char const* const* argv, std::set<ParameterBase*> const& targetParameters) {
	tokenize(argc, argv, [](std::string const&){}, [&](std::string const& argName, std::vector<std::string> const& arguments)
	{
		auto target = std::find_if(targetParameters.begin(), targetParameters.end(), [&](ParameterBase*p){ return p->getArgName() == argName; });
		if (target == targetParameters.end()) {
			return;
		}
		try {
			(*target)->parse(arguments);
		} catch (sargp::parsing::detail::ParseError const& error) {
			throw std::invalid_argument("cannot parse arguments for \"" + argName + "\"");
		}
	});
}

std::string generateHelpString(std::regex const& filter) {
	std::string helpString;

	auto const & commands = detail::CommandRegistry::getInstance().getCommands();
	if (commands.size() != 1) { // if there is more than just the default command
		helpString += "valid commands:\n\n";
		int maxCommandStrLen = 0;
		for (auto it = commands.begin(); it != commands.end(); it = commands.upper_bound(it->first)) {
			if (it->second != &Command::getDefaultCommand()) {
				maxCommandStrLen = std::max(maxCommandStrLen, static_cast<int>(it->first.size())+2); // +2 cause we print two spaces at the beginning
			}
		}
		maxCommandStrLen += 4;
		for (auto it = commands.begin(); it != commands.end(); it = commands.upper_bound(it->first)) {
			if (it->second != &Command::getDefaultCommand()) {
				helpString += "  " + it->first + std::string(maxCommandStrLen - it->first.size(), ' ') + it->second->getDescription() + "\n";
			}
		}
		helpString += "\n";
	}
	for (auto it = commands.begin(); it != commands.end();) {
		int maxArgNameLen = 0;
		bool anyMatch = false;
		auto end = commands.upper_bound(it->first);
		for (auto command = it; command != end; ++command) {
			auto const& params = command->second->getParameters();
			for (auto paramIt = params.begin(); paramIt != params.end(); paramIt = params.upper_bound(paramIt->first)) {
				if (std::regex_match(paramIt->second->getArgName(), filter)) {
					anyMatch = true;
					maxArgNameLen = std::max(maxArgNameLen, static_cast<int>(paramIt->second->getArgName().size()));
				}
			}
		}
		if (anyMatch) {
			maxArgNameLen += 4;
			if (it->second == &Command::getDefaultCommand()) {
				helpString += "\nglobal parameters:\n\n";
			} else {
				helpString += "\nparameters for command " + it->first + ":\n\n";
			}
			for (auto command = it; command != end; ++command) {
				auto const& params = command->second->getParameters();
				for (auto paramIt = params.begin(); paramIt != params.end(); paramIt = params.upper_bound(paramIt->first)) {
					if (std::regex_match(paramIt->second->getArgName(), filter)) {
						ParameterBase* pb = paramIt->second;
						helpString += "--"+pb->getArgName() + std::string(maxArgNameLen - paramIt->first.size(), ' ');
						if (not pb->isSpecified()) {// the difference are the brackets!
							helpString += "(" + pb->stringifyValue() + ")";
						} else {
							helpString += pb->stringifyValue();
						}
						helpString += "\n    "+ pb->describe() + "\n";
					}
				}
			}
		}
		it = end;
	}
	return helpString;
}

std::string generateGroffString() {
	std::string helpString;

	auto const & commands = detail::CommandRegistry::getInstance().getCommands();

	if (commands.size() != 1) { // if there is more than just the default command

		helpString += ".SH COMMANDS\n";

		for (auto it = commands.begin(); it != commands.end(); it = commands.upper_bound(it->first)) {
			if (it->second != &Command::getDefaultCommand()) {
				helpString += ".TP\n\\fB" + it->first + "\\fR\n" + it->second->getDescription() + "\n";
			}
		}
		helpString += "\n";
	}
	bool lastLocal{false};
	for (auto it = commands.begin(); it != commands.end();) {
		bool globalOptions = it->second == &Command::getDefaultCommand();
		if (globalOptions) {
			helpString += ".SH GLOBAL OPTIONS\n";
		}
		if (not lastLocal and not globalOptions) {
			lastLocal = true;
			helpString += ".SH SPECIFIC OPTIONS\n";
		}

		auto end = commands.upper_bound(it->first);
		for (auto command = it; command != end; ++command) {
			if (not globalOptions) {
				helpString += ".SS\n\\fB" + it->first + "\\fR\n";
			}

			auto const& params = command->second->getParameters();
			for (auto paramIt = params.begin(); paramIt != params.end(); paramIt = params.upper_bound(paramIt->first)) {
				helpString += ".TP\n";
				helpString += std::string{"\\fB--"} + paramIt->second->getArgName() + "\\fR\n";
				helpString += paramIt->second->describe() + "\n";
			}
		}
		it = end;
	}
	return helpString;
}

std::set<std::string> getNextArgHint(int argc, char const* const* argv) {
	std::vector<Command*> argProviders = {&Command::getDefaultCommand()};
	std::string lastArgName;
	std::vector<std::string> lastArguments;
	tokenize(argc, argv, [&](std::string const& commandName){
		auto target = detail::CommandRegistry::getInstance().getCommands().equal_range(commandName);
		for (auto t{target.first}; t != target.second; ++t) {
			argProviders.push_back(t->second);
			t->second->setActive(true);
		}
	}, [&](std::string const& argName, std::vector<std::string> const& arguments) {
		for (auto argProvider : argProviders) {
			auto target = argProvider->getParameters().equal_range(argName);
			if (target.first == target.second) {
				continue;
			}
			for (auto t{target.first}; t != target.second; ++t) {
				try {
					t->second->parse(arguments);
				} catch (std::invalid_argument const& error) {}
			}
			lastArgName = argName;
			lastArguments = arguments;
		}
	});
	std::set<std::string> hints;

	if (lastArgName.empty() and argProviders.size() == 1) {
		auto const& commands = detail::CommandRegistry::getInstance().getCommands();
		for (auto it = commands.begin(); it != commands.end(); it = commands.upper_bound(it->first)) {
			hints.emplace(it->first);
		}
	}

	bool canAcceptNextArg = true;
	for (auto argProvider : argProviders) {
		auto target = argProvider->getParameters().equal_range(lastArgName);
		for (auto t{target.first}; t != target.second; ++t) {
			auto [cur_canAcceptNextArg, cur_hints] = t->second->getValueHints(lastArguments);
			canAcceptNextArg &= cur_canAcceptNextArg;
			hints.insert(cur_hints.begin(), cur_hints.end());
		}
	}

	if (canAcceptNextArg) {
		for (auto argProvider : argProviders) {
			for (auto const& p : argProvider->getParameters()) {
				if (not p.second->isSpecified()) {
					hints.insert("--" + p.first);
				}
			}
		}
	}
	return hints;
}

void callCommands() {
	auto const& commands = detail::CommandRegistry::getInstance().getCommands();
	// collect all nondefault commands that can be run
	std::set<Command*> runnableCommands;
	for (auto& command : commands) {
		if (*command.second) {
			runnableCommands.emplace(command.second);
		}
	}
	for (auto cmd : runnableCommands) {
		cmd->callCB();
	}
}

}
