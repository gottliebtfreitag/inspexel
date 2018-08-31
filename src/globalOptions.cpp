#include "globalOptions.h"

#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace {

bool isPrefix(std::string const& _str, std::string const& _prefix) {
	if (_prefix.size() > _str.size()) return false;
	auto res = std::mismatch(_prefix.begin(), _prefix.end(), _str.begin());
	return res.first == _prefix.end();
}

}

auto listDeviceFiles(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>> {
	(void)_str;
	std::set<std::string> res;

	for (auto p : fs::directory_iterator("/dev")) {
		auto fn = p.path().string();
		if (isPrefix(fn, "/dev/ttyUSB")) {
			res.insert(p.path().string());
		}
	}

	if (fs::is_directory("/dev/serial/by-id/")) {
		for (auto p : fs::directory_iterator("/dev/serial/by-id/")) {
			res.insert(p.path().string());
		}
	}

	return {_str.size() > 0, res};
}

