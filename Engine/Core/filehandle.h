#pragma once

#include <fstream>
#include <cstdlib>
#include <string>


enum class OpCode
{
	LoadTileFile,
	SetGrid,
	SetTiles,
	AddTile,
	AddSpriteFact,
	AddEntityFact,
	AddEntity,
	SubscribeMail,
	AddEntControl,
	AddStateControl,
	
};



class FileHandle
{
	std::fstream file;
	std::string currentLine;
	size_t next;

public:

	FileHandle(const char* filePath);

	~FileHandle();


	explicit operator bool()
	{
		if (file)
		{
			return true;
		}

		return false;
	}

	bool eof()
	{
		return file.eof();
	}

	bool endOfLine()
	{
		if (next >= currentLine.size())
		{
			return true;
		}

		return false;
	}

	char getNextCharacter()
	{
		char character;
		file >> character;

		return character;
	}

	bool getLine(std::string& line)
	{
		if (std::getline(file, line))
		{
			return true;
		}

		return false;
	}

	bool advanceLine()
	{
		next = 0;

		if (std::getline(file, currentLine))
		{
			// Skip lines with # as first character
			if (currentLine[0] == '#')
			{
				return advanceLine();
			}

			return true;
		}
		return false;
	}

	std::string getNextString()
	{
		std::string nextString;

		if (endOfLine())
		{
			advanceLine();
		}

		// Read into nextString until a space is found or the end of line is reached
		while (currentLine[next] != ' ' && next < currentLine.size())
		{
			nextString.push_back(currentLine[next]);

			next++;
		}

		// Advance past the space for the next operation
		next++;

		return nextString;
	}

	int getNextInt()
	{

		std::string nextString = getNextString();

		int x = std::stoi(nextString);

		return x;
	}

	int getNextOptionalInt()
	{
		if (!endOfLine())
		{
			return getNextInt();
		}
		else
		{
			return 0;
		}
	}

	std::string getNextOptionalString()
	{
		if (!endOfLine())
		{
			return getNextString();
		}
		else
		{
			return "";
		}
	}

};