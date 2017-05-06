#include "Game.h"

#include <cstdlib>

#include <algorithm>

#define PRINT_WRONG_PATH(path) std::cout << "Wrong path: " << path << std::endl
#define PRINT_MISSING_FILE(msg, path) std::cout << "Missing " << msg << \
				" looking in path: " << path

int main(int argc, char** argv)
{
	std::string filePath("");
	std::string	tempStr("");
	auto error = false;
	auto boardCreateError = true;
	Game battleshipGameManager;
	
	HANDLE dir;
	WIN32_FIND_DATAA fileData;
	std::set<std::string> dlls;
	std::set<std::string> boards;
	char fullPath[MAX_PATH];

	if (argc == 1 || argv[1][0] == '-')
	{
		filePath = _fullpath(fullPath, ".", MAX_PATH);
	}
	else
	{
		filePath = _fullpath(fullPath, argv[1], MAX_PATH);
	}

	tempStr = "\\*.sboard";
	dir = FindFirstFileA((filePath + tempStr).c_str(), &fileData);
	if (dir == INVALID_HANDLE_VALUE)
	{
		PRINT_MISSING_FILE("board file (*.sboard)", filePath);
		error = true;
	}
	else
	{
		do
		{
			tempStr = fileData.cFileName;
			boards.insert(filePath + tempStr);
		} while (FindNextFileA(dir, &fileData));
	}

	if (!error)
	{
		tempStr = *boards.begin();
		std::ifstream boardFile(filePath + tempStr);
		if (!boardFile.is_open())
		{
			PRINT_MISSING_FILE("board file (*.sboard)", filePath);
			error = true;
		}
		else
		{
			if (battleshipGameManager.checkAndCreateBoard(boardFile))
			{
				error = true;
			}

			boardFile.close();
		}
	}

	tempStr = "\\*.dll";
	dir = FindFirstFileA((filePath + tempStr).c_str(), &fileData);
	if (dir != INVALID_HANDLE_VALUE)
	{
		do
		{
			tempStr = fileData.cFileName;
			dlls.insert(filePath + tempStr);
		} while (FindNextFileA(dir, &fileData));
	}

	if (dlls.size() < 2)
	{
		if (error)
			std::cout << std::endl;
		PRINT_MISSING_FILE("an algorithm (dll) file", filePath);
		error = true;
	}

	if (error)
		return EXIT_FAILURE;

	if (!(battleshipGameManager.loadAndInitPlayersFromDLL(filePath, dlls)))
	{
		return EXIT_FAILURE;
	}

	battleshipGameManager.checkParameters(argv, argv + argc);

	/* Playing game */
	while (battleshipGameManager.playMove() != GameState::GAME_OVER) {}

	return EXIT_SUCCESS;
}