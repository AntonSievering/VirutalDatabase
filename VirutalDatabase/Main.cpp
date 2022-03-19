#include "VirtualDatabase.hpp"

int main()
{
	VirtualFilesystem fs("output.fs");
	fs.addFile("hi", "bye");
}
