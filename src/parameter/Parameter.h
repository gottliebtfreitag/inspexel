#pragma once

#include "utils/Singleton.h"
#include "ParameterParsing.h"

#include <regex>
#include <map>
#include <set>
#include <vector>
#include <functional>
#include <numeric>

namespace parameter
{

struct Command;

namespace detail
{

template<typename T>
struct UniqueStorageManager final : Singleton<UniqueStorageManager<T>> {
	using StorageT = std::map<std::string, T>;
	StorageT storage;
};

struct CommandRegistry : Singleton<CommandRegistry> {
private:
	std::multimap<std::string, Command*> _commands;
public:

	void registerCommand(std::string const& name, Command* command) {
		_commands.emplace(name, command);
	}
	void deregisterCommand(std::string const& name, Command* command) {
		_commands.emplace(name, command);
	}

	auto getCommands() const -> decltype(_commands) const& {
		return _commands;
	}
};

}

struct Command;
Command& getDefaultCommand();

struct ParameterBase {
	using Callback     = std::function<void()>;
	using DescribeFunc = std::function<std::string()>;
	/**
	 *  given some strings that might be passed as arguments to the Parameter
	 *  can the amount of arguments be accepted (that is in terms of bash completion can other arguments be started)
	 *  and what values can be proposed to the user
	 */
	using ValueHintFunc = std::function<std::pair<bool, std::set<std::string>>(std::vector<std::string> const&)>;
	virtual ~ParameterBase();

	virtual void parse([[maybe_unused]] std::vector<std::string> const&  args) {
		_valSpecified = true;
		if (_cb) {
			_cb();
		}
	};
	virtual std::string stringifyValue() const = 0;

	virtual std::string describe() const {
		return _describeFunc();
	}

	virtual auto getValueHints(std::vector<std::string> const& args) const -> std::pair<bool, std::set<std::string>> {
		if (_hintFunc) {
			return _hintFunc(args);
		}
		return std::make_pair<bool, std::set<std::string>>(not args.empty(), {});
	}

	auto getArgName() const -> std::string const& {
		return _argName;
	}

	/**
	 * return true iff the Parameter was set (by any means)
	 * even if the contained value is the default value
	 */
	bool isSpecified() const { return _valSpecified; };

protected:
	ParameterBase(std::string const& argName, DescribeFunc const& describeFunc, Callback cb, ValueHintFunc const& hintFunc, Command& command);

	std::string _argName;
	DescribeFunc _describeFunc;
	Callback _cb;
	ValueHintFunc _hintFunc;
	Command& _command;
	bool _valSpecified {false};
};

template<typename T>
struct TypedParameter : ParameterBase {
protected:
	using SuperClass = ParameterBase;
	using StorageManger = detail::UniqueStorageManager<T>;
	using ValRefT = typename StorageManger::StorageT::iterator;
	using value_type = T;

	ValRefT _val;
public:
	TypedParameter(T const& defaultVal, std::string const& argName, std::string const& description, Callback cb=Callback{}, ValueHintFunc hintFunc=ValueHintFunc{}, Command& command=getDefaultCommand())
	: TypedParameter(defaultVal, argName, [=]{return description;}, cb, hintFunc, command)
	{}
	TypedParameter(T const& defaultVal, std::string const& argName, DescribeFunc const& description, Callback cb=Callback{}, ValueHintFunc hintFunc=ValueHintFunc{}, Command& command=getDefaultCommand())
	: SuperClass(argName, description, cb, hintFunc, command)
	, _val(StorageManger::getInstance().storage.emplace(argName, defaultVal).first)
	{}

	operator T const&() const {
		return _val->second;
	}

	T const& get() const {
		return _val->second;
	}

	T& get() {
		return _val->second;
	}
};

// an intermediate type broker to inject type specific specializations eg. for rendering
template<typename T>
struct SpeciallyTypedParameter : TypedParameter<T> {
	using value_type = T;
	using SuperClass = TypedParameter<T>;
	using SuperClass::SuperClass;

	void parse(std::vector<std::string> const& args) override {
		SuperClass::_val->second = parsing::parse<T>(args);
		SuperClass::parse(args);
	}
};

template<>
struct SpeciallyTypedParameter<bool> : TypedParameter<bool> {
	using SuperClass = TypedParameter<bool>;
	using value_type = SuperClass::value_type;
	using SuperClass::SuperClass;

	void parse(std::vector<std::string> const& args) override {
		SuperClass::_val->second = parsing::parse<value_type>(args);
		SuperClass::parse(args);
	}

	auto getValueHints(std::vector<std::string> const& args) const -> std::pair<bool, std::set<std::string>> override {
		if (SuperClass::_hintFunc) {
			return SuperClass::_hintFunc(args);
		}
		if (args.empty() or (args.size() == 1 and args[0] != "true" and args[0] != "false")) {
			return std::make_pair<bool, std::set<std::string>>(true, {"true", "false"});
		}
		return std::make_pair<bool, std::set<std::string>>(true, {});
	}
};

template<typename T>
struct SpeciallyTypedParameter<std::optional<T>> : TypedParameter<std::optional<T>> {
	using SuperClass = TypedParameter<std::optional<T>>;
	using value_type = typename SuperClass::value_type;
	using SuperClass::SuperClass;

	void parse(std::vector<std::string> const& args) override {
		SuperClass::_val->second = parsing::parse<value_type>(args);
		SuperClass::parse(args);
	}

	auto getValueHints(std::vector<std::string> const& args) const -> std::pair<bool, std::set<std::string>> override {
		if (SuperClass::_hintFunc) {
			return SuperClass::_hintFunc(args);
		}
		return std::make_pair<bool, std::set<std::string>>(true, {});
	}
};

template<typename T>
struct SpeciallyTypedParameter<std::vector<T>> : TypedParameter<std::vector<T>> {
	using SuperClass = TypedParameter<std::vector<T>>;
	using value_type = typename SuperClass::value_type;
	using SuperClass::SuperClass;

	void parse(std::vector<std::string> const& args) override {
		SuperClass::_val->second = parsing::parse<value_type>(args);
		SuperClass::parse(args);
	}

	auto getValueHints(std::vector<std::string> const& args) const -> std::pair<bool, std::set<std::string>> override {
		if (SuperClass::_hintFunc) {
			return SuperClass::_hintFunc(args);
		}
		return std::make_pair<bool, std::set<std::string>>(true, {});
	}
};
template<typename T>
struct SpeciallyTypedParameter<std::set<T>> : TypedParameter<std::set<T>> {
	using SuperClass = TypedParameter<std::set<T>>;
	using value_type = typename SuperClass::value_type;
	using SuperClass::SuperClass;

	void parse(std::vector<std::string> const& args) override {
		SuperClass::_val->second = parsing::parse<value_type>(args);
		SuperClass::parse(args);
	}

	auto getValueHints(std::vector<std::string> const& args) const -> std::pair<bool, std::set<std::string>> override {
		if (SuperClass::_hintFunc) {
			return SuperClass::_hintFunc(args);
		}
		return std::make_pair<bool, std::set<std::string>>(true, {});
	}
};

template<typename T>
struct Parameter : SpeciallyTypedParameter<T> {
	using SuperClass = SpeciallyTypedParameter<T>;
	using SuperClass::SuperClass;

	std::string stringifyValue() const override {
		return parsing::stringify(SuperClass::_val->second);
	}
};

struct Flag : Parameter<bool> {
	using SuperClass = Parameter<bool>;
	using Callback = typename SuperClass::Callback;
	Flag(std::string const& argName, std::string const& description, Callback cb=Callback{}, Command& command=getDefaultCommand())
	: Parameter<bool>(false, argName, description, cb, ValueHintFunc{}, command)
	{}

	std::string stringifyValue() const override {
		return "";
	}
};

template<typename T>
struct Choice : TypedParameter<T> {
	using SuperClass = TypedParameter<T>;
	using Callback = typename SuperClass::Callback;
private:
	std::map<std::string, T> _name2ValMap;
public:
	Choice(T const& defaultVal, std::string const& argName, std::map<std::string, T> const& namedValues, std::string const& description, Callback cb=Callback{}, Command& command=getDefaultCommand())
	: SuperClass(defaultVal, argName, description, cb, command)
	, _name2ValMap(namedValues)
	{}

	void parse(std::vector<std::string> const& args) override {
		if (args.size() != 1) {
			throw parsing::detail::ParseError("a choice must be given exactly one value");
		}
		auto it = _name2ValMap.find(args[0]);
		if (it == _name2ValMap.end()) {
			throw parsing::detail::ParseError("cannot interpret " + args[0] + " as a valid value for " + SuperClass::getArgName());
		}
		SuperClass::_val->second = it->second;
		SuperClass::parse(args);
	}

	std::string stringifyValue() const override {
		for (auto const& n2v : _name2ValMap) {
			if (n2v.second == SuperClass::_val->second) {
				return n2v.first;
			}
		}
		return "";
	}

	auto getValueHints(std::vector<std::string> const& args) const -> std::pair<bool, std::set<std::string>> override {
		if (SuperClass::_hintFunc) {
			return SuperClass::_hintFunc(args);
		}
		std::set<std::string> names;
		if (args.empty()) {
			for (auto const& n2v : _name2ValMap) { names.emplace(n2v.first); };
		} else if (args.size() == 0) {
			if (_name2ValMap.find(args[0]) == _name2ValMap.end()) {
				for (auto const& n2v : _name2ValMap) { names.emplace(n2v.first); };
			}
		}
		return std::make_pair<bool, std::set<std::string>>(args.size() == 1, std::move(names));
	}

	std::string describe() const override {
		std::string description = "valid values: {";
		if (not _name2ValMap.empty()) {
			description += std::accumulate(std::next(_name2ValMap.begin()), _name2ValMap.end(), _name2ValMap.begin()->first, [](std::string const& left, std::pair<std::string, T> const& p) {
				return left + " " + p.first;
			});
		}
		description += "}\n\t";
		description += SuperClass::describe();
		return description;
	}
};

struct Section {
private:
	std::string _name;
	Command& _command;
public:
	Section(std::string const& name, Command& command = getDefaultCommand())
	: _name(name + ".")
	, _command(command)
	{}

	template<typename T, typename... Args>
	[[nodiscard]] auto Parameter(T const& defaultVal, std::string const& argName, Args &&... args) const -> Parameter<T> {
		return ::parameter::Parameter<T>{defaultVal, _name + argName, std::forward<Args>(args)...};
	}
	template<typename... Args>
	[[nodiscard]] auto Flag(std::string const& argName, Args &&... args) const -> Flag {
		return ::parameter::Flag{_name + argName, std::forward<Args>(args)...};
	}
	template<typename T, typename... Args>
	[[nodiscard]] auto Choice(T const& defaultVal, std::string const& argName, std::map<std::string, T> const& namedValues, Args &&... args) const -> Choice<T> {
		return ::parameter::Choice<T>{defaultVal, _name + argName, namedValues, std::forward<Args>(args)...};
	}
};


struct Command {
	using Registry = detail::CommandRegistry;
	using Callback     = ParameterBase::Callback;
	using DescribeFunc = ParameterBase::DescribeFunc;
	using ValueHintFunc = ParameterBase::ValueHintFunc;

private:
	std::string _name;
	std::string _description;
	Callback    _cb;
	bool        _isActive {false};
	std::multimap<std::string, ParameterBase*> parameters;
public:

	Command(std::string const& name, std::string const& description, Callback const& cb=Callback{}) : _name(name), _description(description), _cb(cb) {
		Registry::getInstance().registerCommand(_name, this);
	}
	~Command() {
		Registry::getInstance().deregisterCommand(_name, this);
	}

	auto getDescription() const -> decltype(_description) const& {
		return _description;
	}

	void registerParameter(std::string const& name, ParameterBase* parameter) {
		parameters.emplace(name, parameter);
	}
	void deregisterParameter(std::string const& name, ParameterBase* parameter) {
		parameters.emplace(name, parameter);
	}
	auto getParameters() -> decltype(parameters) {
		return parameters;
	}

	void setActive(bool active) {
		_isActive = active;
	}

	operator bool() const {
		return _isActive;
	}

	void callCB() {
		if (_cb) {
			_cb();
		}
	}

	static Command& getDefaultCommand() {
		static Command instance{"", ""};
		return instance;
	}

	// parameters that are enabled if this Command is active
	template<typename T>
	[[nodiscard]] auto Parameter(T const& defaultVal, std::string const& argName, std::string const& description, Callback cb=Callback{}, ValueHintFunc hintFunc=ValueHintFunc{}) -> ::parameter::Parameter<T> {
		return ::parameter::Parameter<T>{defaultVal, argName, description, cb, hintFunc, *this};
	}
	template<typename T>
	[[nodiscard]] auto Parameter(T const& defaultVal, std::string const& argName, DescribeFunc const& description, Callback cb=Callback{}, ValueHintFunc hintFunc=ValueHintFunc{}, Command& command=getDefaultCommand()) -> ::parameter::Parameter<T> {
		return ::parameter::Parameter<T>{defaultVal, argName, description, cb, hintFunc, *this};
	}

	[[nodiscard]] auto Flag(std::string const& argName, std::string const& description, Callback cb=Callback{}) -> ::parameter::Flag {
		return ::parameter::Flag{argName, description, cb, *this};
	}
	template<typename T, typename... Args>
	[[nodiscard]] auto Choice(T const& defaultVal, std::string const& argName, std::map<std::string, T> const& namedValues, std::string const& description, Callback cb=Callback{}) -> ::parameter::Choice<T> {
		return ::parameter::Choice<T>{defaultVal, argName, namedValues, description, cb, *this};
	}
	[[nodiscard]] auto Section(std::string const& name) -> Section {
		return ::parameter::Section(name, *this);
	}
};

}


