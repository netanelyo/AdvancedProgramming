#include "Game.h"

#include <cstdlib>

#define PRINT_WRONG_PATH(path) std::cout << "Wrong path: " << path << std::endl
#define PRINT_MISSING_FILE(msg, path) std::cout << "Missing " << msg << \
				" looking in path: " << path << std::endl
#define SBOARD 0
#define ATTACK_A 1
#define ATTACK_B 2

/**
 * Checks if filename has a suitable extension (.attack-a/.attack-b/.sboard),
 * if it does - assigns to files in the matching index.
 * 
 * @param files - an array of strings to be assigned with input files (if exist)
 * @param filename - a candidte to be an input file
 */
void matchFileExtensionAndAssign(std::string files[3], const std::string& filename)
{
	auto extIndex = filename.find_last_of('.'); /* Index of extension in substring */
	auto len = filename.length();

	/* Compares extension to one of the three mentioned above */
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

/**
 * @return std::string - matching message according to missing file
 */
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
	std::string cmd = "2>NUL dir ";
	std::string filePath("");
	std::string filesArr[3] = { "", "", "" };
	std::string	tempStr("");
	bool missing = false;
	bool workingDir = false;

	if (argc == 2)
	{
		cmd += "\"";
		cmd += argv[1];
		cmd += "\"";

		filePath = argv[1];
	}
	else if (argc == 1) /* No path in CMD-line args */
	{
		filePath = ".";
		workingDir = true;
	}
	cmd += " /b /a-d > files.txt";

	/* Get files list in path folder */
	rc = system(cmd.c_str()); /* Listing files in path to "files.txt" file */
	if (rc)
	{
		if (!workingDir)
			PRINT_WRONG_PATH(filePath);
		else
			PRINT_WRONG_PATH("Working Directory");

		return EXIT_FAILURE;
	}

	/* Reading "files.txt" to get input filenames */
	std::ifstream filesStream("files.txt");
	while (std::getline(filesStream, tempStr))
	{
		matchFileExtensionAndAssign(filesArr, tempStr);
	}
	filesStream.close();

	/* Checking if there are any missing input files */
	for (int i = 0; i < 3; ++i)
	{
		if (filesArr[i].empty())
		{
			if (workingDir)
				filePath = "Working Directory";

			PRINT_MISSING_FILE(getProperMessage(i), filePath);
			missing = true;
		}
	}

	if (missing)
		return EXIT_FAILURE;

	/* Opens board and moves files */
	std::ifstream boardFile(filePath + "\\" + filesArr[SBOARD]);
	Game battleshipGameManager(filePath + "\\" + filesArr[ATTACK_A],
		filePath + "\\" + filesArr[ATTACK_B]);
	if (battleshipGameManager.checkAndCreateBoard(boardFile))
	{
		boardFile.close();
		return EXIT_FAILURE;
	}

	boardFile.close();

	/* Playing game */
	while (battleshipGameManager.playMove() != GameState::GAME_OVER) {}

	return EXIT_SUCCESS;
}