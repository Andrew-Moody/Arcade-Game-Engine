#include "filehandle.h"

#include <fstream>
#include <iostream>

FileHandle::FileHandle(const char* filePath)
{
	file.open(filePath);

	next = 0;

	if (!file)
	{
		std::cout << "Unable to load file: " << filePath << std::endl;
	}
}


FileHandle::~FileHandle()
{
	file.close();
}