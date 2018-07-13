#pragma once

#include <set>
#include <functional>
#include <memory>
#include <mutex>

template<typename... Args>
struct CanCallBack;

namespace detail
{

template<typename... Args>
struct Callback {
	struct CallbackInfo {
		std::mutex mutex;
		CanCallBack<Args...>* owner{};
		std::function<void(Args...)> cb;
		~CallbackInfo();
	};
	std::unique_ptr<CallbackInfo> data{std::make_unique<CallbackInfo>()};
};

}

template<typename... Args>
struct CanCallBack {
	using Callback = detail::Callback<Args...>;
private:
	using CallbackInfo = typename Callback::CallbackInfo;
	std::set<CallbackInfo*> infos;
	std::recursive_mutex mutex;

protected:
	void callCallbacks(Args... args) {
		std::lock_guard lock(mutex);
		for (auto info : infos) {
			info->cb(args...);
		}
	}

	~CanCallBack() {
		std::lock_guard lock(mutex);
		for (auto info : infos) {
			std::lock_guard lock2(info->mutex);
			info->owner = nullptr;
		}
	}
public:
	template<typename Func>
	[[nodiscard]] Callback addCallback(Func&& cb) {
		Callback ret;
		ret.data->cb    = std::forward<Func>(cb);
		ret.data->owner = this;
		std::lock_guard lock(mutex);
		infos.emplace(ret.data.get());
		return ret;
	}

	void deregisterCB(CallbackInfo* info) {
		std::lock_guard lock(mutex);
		infos.erase(info);
	}
};

namespace detail
{

template<typename... Args>
Callback<Args...>::CallbackInfo::~CallbackInfo() {
	std::lock_guard lock{mutex};
	if (owner) {
		owner->deregisterCB(this);
	}
}

}


