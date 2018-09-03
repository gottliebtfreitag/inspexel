#include "FileDescriptor.h"

#include <map>
#include <optional>
#include <string>

namespace simplyfile {

struct INotify : FileDescriptor {
	using FileDescriptor::FileDescriptor;
	INotify();

	enum class Event : uint32_t {
		None = 0x00,
		ChildAccess = 0x01, // Accessing a file (read)
		ChildCreate = 0x02, // A file is being created (touch)
		ChildDelete = 0x04, // file being deleted (rm)
		ChildModify = 0x08, // (write)
		SelfDelete  = 0x10, // the file itself was deleted (rm)
	};
	// check inotify for mask meaning
	void watch(std::string const& _path, Event event);

	std::map<int, std::tuple<std::string, int32_t>> mIDs;

	struct Result {
		std::string path;
		std::string file;
	};
};

inline auto operator|(INotify::Event e1, INotify::Event e2) -> INotify::Event {
	using UT = std::underlying_type_t<INotify::Event>;
	return INotify::Event{UT(e1) | UT(e2)};
}
inline auto operator&(INotify::Event e1, INotify::Event e2) -> INotify::Event {
	using UT = std::underlying_type_t<INotify::Event>;
	return INotify::Event(UT(e1) & UT(e2));
}
inline auto operator !=(INotify::Event e1, std::underlying_type_t<INotify::Event> val) -> bool {
	using UT = std::underlying_type_t<INotify::Event>;
	return UT(e1) != val;
}

[[nodiscard]] auto read(INotify const& fd) -> std::optional<INotify::Result>;

}
