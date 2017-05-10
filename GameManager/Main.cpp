#include "Game.h"

#include <fstream>

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

	/*gets directory path from cmd arguments, if exists. Otherwise, uses our current working directory*/
	if (argc == 1 || argv[1][0] == '-')
	{
		_fullpath(fullPath, ".", MAX_PATH);
		tempStr = "Working Directory";
	}
	else
	{
		_fullpath(fullPath, argv[1], MAX_PATH);
		tempStr = argv[1];
	}

	if (fullPath == nullptr)
	{
		std::cout << "Wrong path: " << tempStr;
		return EXIT_FAILURE;
	}

	filePath = fullPath;
	filePath += "\\";

	/*looks for board file*/
	tempStr = "\\*.sboard";
	dir = FindFirstFileA((filePath + tempStr).c_str(), &fileData);
	if (dir == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() != ERROR_FILE_NOT_FOUND)
		{
			std::cout << "Wrong path: " << fullPath;
			return EXIT_FAILURE;
		}
		PRINT_MISSING_FILE("board file (*.sboard)", filePath);
		error = true;
	}
	else
	{
		do
		{
			tempStr = fileData.cFileName;
			boards.insert(tempStr);
		} while (FindNextFileA(dir, &fileData));
	}
	/*opens the board file and parses the board (in case a board file was found)*/
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
	/*looks for dll files*/
	tempStr = "\\*.dll";
	dir = FindFirstFileA((filePath + tempStr).c_str(), &fileData);
	if (dir != INVALID_HANDLE_VALUE)
	{
		do
		{
			tempStr = fileData.cFileName;
			dlls.insert(tempStr);
		} while (FindNextFileA(dir, &fileData));
	}
	/*checks we found at least 2 dll files*/
	if (dlls.size() < 2)
	{
		if (error)
			std::cout << std::endl;
		PRINT_MISSING_FILE("an algorithm (dll) file", filePath);
		error = true;
	}
	/*if any error occurred above, we exit the program*/
	if (error)
		return EXIT_FAILURE;
	/*loads dll files, sets board for players and initializes each player*/
	if (!(battleshipGameManager.loadAndInitPlayersFromDLL(filePath, dlls)))
	{
		return EXIT_FAILURE;
	}
	/*checks if we got extra paramaters from cmd*/
	battleshipGameManager.checkParameters(argv, argv + argc);
	/*initializes game animiation if necessary*/
	battleshipGameManager.initializeGame();

	/* Plays game */
	while (battleshipGameManager.playMove() != GameState::GAME_OVER) {}

	return EXIT_SUCCESS;
}