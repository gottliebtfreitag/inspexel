#pragma once

#include "FileDescriptor.h"

#include <chrono>
#include <memory>
#include <string>
#include <sys/epoll.h>

namespace simplyfile
{

namespace detail
{
template<typename T>
struct unique_func;

template<typename Res, typename... Args>
struct unique_func<Res(Args...)> {
private:
    struct PimplBase {
        PimplBase() = default;
        virtual ~PimplBase() = default;
        virtual Res invoke(Args &&... args) = 0;
        virtual std::type_info const& target_type() const = 0;
    };

    template<typename Functor>
    struct TypedPimpl : PimplBase {
        Functor ftor;
        TypedPimpl(Functor _ftor) : ftor{std::move(_ftor)} {}
        Res invoke(Args &&... args) override {
            return std::invoke(ftor, std::forward<Args>(args)...);
        }
        std::type_info const& target_type() const override {
            return typeid(Functor);
        }
    };
    std::unique_ptr<PimplBase> pimpl;
public:
    template<typename Functor>
    unique_func(Functor functor) : pimpl{std::make_unique<TypedPimpl<Functor>>(std::move(functor))} {}
    
    unique_func() = default;
    unique_func(unique_func&&) = default;
    unique_func& operator=(unique_func&&) = default;

    Res operator()(Args&& ... args) {
        return pimpl->invoke(std::forward<Args>(args)...);
    }

    std::type_info const& target_type() const {
        return pimpl->target_type();
    }
};

template<typename>
struct __function_guide_helper {};

template<typename _Res, typename _Tp, bool _Nx, typename... _Args>
struct __function_guide_helper<_Res (_Tp::*) (_Args...) noexcept(_Nx)>
{ using type = _Res(_Args...); };

template<typename _Res, typename _Tp, bool _Nx, typename... _Args>
struct __function_guide_helper<_Res (_Tp::*) (_Args...) & noexcept(_Nx)>
{ using type = _Res(_Args...); };

template<typename _Res, typename _Tp, bool _Nx, typename... _Args>
struct __function_guide_helper<_Res (_Tp::*) (_Args...) const noexcept(_Nx)>
{ using type = _Res(_Args...); };

template<typename _Res, typename _Tp, bool _Nx, typename... _Args>
struct __function_guide_helper<_Res (_Tp::*) (_Args...) const & noexcept(_Nx)>
{ using type = _Res(_Args...); };

template<typename _Functor, typename _Signature = typename detail::__function_guide_helper<decltype(&_Functor::operator())>::type>
unique_func(_Functor) -> unique_func<_Signature>;

}

struct Epoll : FileDescriptor {
	using Callback = detail::unique_func<void(int)>;

	Epoll();
	Epoll(Epoll &&other) noexcept;
	Epoll& operator=(Epoll &&rhs) noexcept;
	~Epoll();

	void addFD(int fd, Callback callback, int epollFlags = EPOLLIN|EPOLLET, std::string const& name="");
	void modFD(int fd, int epollFlags = EPOLLIN|EPOLLET);
	void rmFD(int fd, bool blocking);

	void work(int maxEvents=1, int timeout_ms=-1) {
		dispatch(wait(maxEvents, timeout_ms));
	}

	// call epoll_wait internally and return the list of events
	std::vector<struct epoll_event> wait(int maxEvents=32, int timeout_ms=-1);

	// call all callbacks of the event list
	void dispatch(std::vector<struct epoll_event> const&);

	// wakes up count thread that is calling wait
	void wakeup(uint64_t count=1) noexcept;

	struct RuntimeInfo {
		std::chrono::nanoseconds accumulatedRuntime {0};
		int64_t numExecutions {0};
		RuntimeInfo& operator+=(RuntimeInfo const& rhs) {
			accumulatedRuntime += rhs.accumulatedRuntime;
			numExecutions += rhs.numExecutions;
			return *this;
		}
		RuntimeInfo& operator-=(RuntimeInfo const& rhs) {
			accumulatedRuntime -= rhs.accumulatedRuntime;
			numExecutions -= rhs.numExecutions;
			return *this;
		}

		RuntimeInfo operator+(RuntimeInfo const& rhs) const {
			RuntimeInfo info{*this};
			info.accumulatedRuntime += rhs.accumulatedRuntime;
			info.numExecutions += rhs.numExecutions;
			return info;
		}

		RuntimeInfo operator-(RuntimeInfo const& rhs) const {
			RuntimeInfo info{*this};
			info.accumulatedRuntime -= rhs.accumulatedRuntime;
			info.numExecutions -= rhs.numExecutions;
			return info;
		}
	};
	std::map<std::string, RuntimeInfo> getRuntimes() const;
private:
	struct Pimpl;
	std::unique_ptr<Pimpl> pimpl;
};

}
