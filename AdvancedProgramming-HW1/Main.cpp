#include "Game.h"

#include <iostream>
#include <cstdlib>

#define PRINT_WRONG_PATH(path) std::cout << "Wrong path: " << path << std::endl
#define PRINT_MISSING_FILE(msg, path) std::cout << "Missing " << msg << \
				" looking in path: " << path << std::endl
#define ATTACK_A 0
#define ATTACK_B 1
#define SBOARD 2

void matchFileExtensionAndAssign(std::string files[3], const std::string& filename)
{
	const auto extIndex = filename.find_last_of('.');
	std::string::size_type len = filename.length();
	if (!filename.compare(extIndex, len, ".attack-a"))
	{
		files[ATTACK_A] = filename;
	}
	else if (!filename.compare(extIndex, len, ".attack-b"))
	{
		files[ATTACK_B] = filename;
	}
	else if (!filename.compare(extIndex, len, ".sboard"))
	{
		files[SBOARD] = filename;
	}
}

std::string getProperMessage(int i)
{
	switch (i)
	{
	case ATTACK_A:
		return "attack file for player A (*.attack-a)";
	case ATTACK_B:
		return "attack file for player B (*.attack-b)";
	case SBOARD:
		return "board file (*.sboard)";
	default:
		return "";
	}
}

int main(int argc, char** argv)
{
	int rc;
	std::string cmd = "dir ";
	std::string filePath("");
	std::string filesArr[3] = { "", "", "" };
	std::string	tempStr("");
	bool missing = false;

	if (argc == 2)
	{
		cmd += "\"";
		cmd += argv[1];
		cmd += "\"";
		
		filePath = argv[1];
	}
	cmd += " /b /a-d > files.txt";

	/* Get files list in path folder */
	rc = system(cmd.c_str());
	if (rc)
	{
		if (!filePath.empty())
			PRINT_WRONG_PATH(filePath);
		else
			PRINT_WRONG_PATH("Working Directory");

		return EXIT_FAILURE;
	}
	
	std::ifstream filesStream("files.txt");

	while (!std::getline(filesStream, tempStr).eof())
	{
		matchFileExtensionAndAssign(filesArr, tempStr);
	}

	for (int i = 0; i < 3; ++i)
	{
		if (filesArr[i].empty())
		{
			if (filePath.empty())
				filePath = "Working Directory";

			PRINT_MISSING_FILE(getProperMessage(i), filePath);
			missing = true;
		}
	}

	if (missing)
		return EXIT_FAILURE;

	std::ifstream boardFile(filePath + "\\" + filesArr[SBOARD]);
	Game battleshipGameManager(filePath + "\\" + filesArr[ATTACK_A],
								filePath + "\\" + filesArr[ATTACK_B]);
	if (battleshipGameManager.checkAndCreateBoard(boardFile))
	{
		boardFile.close();
		return EXIT_FAILURE;
	}

	battleshipGameManager.createBoardsForPlayers(); //TODO leave it here?

	boardFile.close();
	
	while (battleshipGameManager.playMove() != GameState::GAME_OVER) {}

	return EXIT_SUCCESS;
}
