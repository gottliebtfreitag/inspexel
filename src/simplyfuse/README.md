# simplyfuse

simplyfuse is a very simple C++ wrapper around fuse (the filesystem).

here is a minimal example about how to use it:

~~~
#include <simplyfuse/FuseFS.h>

int main() {
	// create a fuseFS and mount it at "mountpoint" (this will create the mountpoint directory if necessary
	simplyfuse::FuseFS fs{"mountpoint"};

	// a simple read writable file containing a std::string
	// files will automatically unregister themselves upon destruction
	simplyfuse::SimpleRWFile myRWFile{"Hello from a read writable file\n"};
	simplyfuse::SimpleROFile myROFile{"Hello from a read only file\n"};

	// reflect the file "myFile" under /my_file
	fs.registerFile("/my_file", myRWFile);
	// files can be registered multiple times
	// reflect the file "myFile" also under /some_path/my_file (intermediate directories will be create automatically)
	fs.registerFile("/some_path/my_file", myRWFile);

	fs.registerFile("/my_read_only_file", myROFile);

	// create a directory in the filesystem (intermediate directories will be created automatically)
	fs.mkdir("/some/random/path");
	// remove "ramdom/path" from the above path (rmdir is always recursive)
	fs.rmdir("/some/random");

	while (true) {
		// loop lets the underlying libfuse handle a single command and then returns
		fs.loop();
	}
	return 0;
}
~~~

there is actually not much to say about simplyfuse...
