#include "MotorMetaInfo.h"

#include <cmath>


namespace dynamixel::meta {


auto getMotorInfos() -> std::vector<MotorInfo> const& {
	static auto data = std::vector<MotorInfo>{};
	static bool firstRun{true};
	if (firstRun) {
		firstRun = false;

		meta::forAllLayoutTypes([&](auto const& _info) {
			using Info = std::decay_t<decltype(_info)>;
			auto& defaults = Info::getDefaults();
			for (auto [id, d] : defaults) {
				MotorInfo info;
				info.modelNumber = d.modelNumber;
				info.layout      = d.layout;

				info.shortName   = d.shortName;
				info.motorNames  = d.motorNames;

				info.converterFunctions = d.converterFunctions;
				data.push_back(info);
			}
		});
	}
	return data;
}
auto getMotorInfo(uint16_t _modelNumber) -> MotorInfo const* {
	for (auto const& motorInfo : getMotorInfos()) {
		if (motorInfo.modelNumber == _modelNumber) {
			return &motorInfo;
		}
	}
	return nullptr;

}
auto getMotorInfo(std::string const& _name) -> MotorInfo const* {
	for (auto const& motorInfo : getMotorInfos()) {
		if (motorInfo.shortName == _name) {
			return &motorInfo;
		}
		for (auto const& _other : motorInfo.motorNames) {
			if (_other == _name) {
				return &motorInfo;
			}
		}
	}
	return nullptr;
}
}
