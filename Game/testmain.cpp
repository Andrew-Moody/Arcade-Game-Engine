
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#include <iostream>

int main()
{
	
	std::ifstream filehandle;
	filehandle.open("C:/dev/Programming 2D Games/Game/testtext.txt");

	if (!filehandle)
	{
		std::cout << "error could not open file\n";
		return -1;
	}

	std::string line;

	std::vector<int> grid;

	int n;

	filehandle >> n;

	while (filehandle)
	{
		grid.push_back(n);
		
		filehandle >> n;
	}

	for (int i = 0; i < grid.size(); ++i)
	{
		std::cout << grid[i];
	}

	


}