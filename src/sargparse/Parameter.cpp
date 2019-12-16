#include "Parameter.h"

#include <algorithm>

namespace sargp
{

ParameterBase::ParameterBase(std::string const& argName, DescribeFunc const& describeFunc, Callback cb, ValueHintFunc const& hintFunc, Command& command)
	: _argName(argName)
	, _describeFunc(describeFunc)
	, _cb(cb)
	, _hintFunc(hintFunc)
	, _command(command)
{
	_command.registerParameter(_argName, *this);
}

ParameterBase::~ParameterBase() {
	_command.deregisterParameter(_argName, *this);
}

Command& getDefaultCommand() {
	return Command::getDefaultCommand();
}


}

