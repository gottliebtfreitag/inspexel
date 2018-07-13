#pragma once

template<typename Func>
struct Finally final {
	Finally(Func && f) : _f(f) {}
	~Finally() {_f();}
private:
	Func _f;
};

template<typename Func>
Finally<Func> makeFinally(Func && f) { return Finally<Func>(std::move(f)); }


