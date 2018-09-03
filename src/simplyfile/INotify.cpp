#include "INotify.h"

#include <sys/inotify.h>
#include <linux/limits.h>

#include <iostream>

namespace simplyfile {

INotify::INotify()
	: FileDescriptor(::inotify_init())
{}

void INotify::watch(std::string const& _path, Event event) {
	uint32_t mask{0};
	if ((event & Event::ChildAccess) != 0x00) mask = mask | IN_ACCESS;
	if ((event & Event::ChildCreate) != 0x00) mask = mask | IN_CREATE;
	if ((event & Event::ChildDelete) != 0x00) mask = mask | IN_DELETE;
	if ((event & Event::ChildModify) != 0x00) mask = mask | IN_DELETE_SELF;
	if ((event & Event::SelfDelete)  != 0x00) mask = mask | IN_MODIFY;

	std::cout << int(*this) << "\n";
	int id = inotify_add_watch(*this, _path.c_str(), mask);
	mIDs[id] = std::make_tuple(_path, mask);
}

auto read(INotify const& fd) -> std::optional<INotify::Result> {
	auto maxSize = sizeof(inotify_event) + NAME_MAX + 1;
	auto buffer = read(fd, maxSize, true);
	inotify_event const& event = (*(inotify_event const*)buffer.data());

	INotify::Result res;
	auto [path, mask] = fd.mIDs.at(event.wd);

	if ((mask | event.mask) == 0x00) {
		return std::nullopt;
	}

	res.path = path;
	if (event.len > 0) {
		res.file = event.name;
	}

	return res;
}


}

