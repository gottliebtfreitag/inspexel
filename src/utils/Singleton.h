#pragma once


template<typename T>
struct Singleton {
	static T& getInstance() {
		static T instance;
		return instance;
	}
protected:
	Singleton() = default;
	~Singleton() = default;
};

