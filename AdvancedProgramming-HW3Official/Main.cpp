#include "MainUtils.h"
#include "TournamentManager.h"

#define PRINT_WRONG_PATH(path) std::cout << "Wrong path: " << path << std::endl
#define PRINT_MISSING_FILE(msg, path) std::cout << "Missing " << msg << \
				" looking in path: " << path


const std::string MainUtils::DEFAULT_LOGFILE_NAME = "game.log";
const std::string MainUtils::CONFIG_END = ".config";

int main(int argc, char** argv)
{
	std::string dirPath(".");
	auto numOfThreads = MainUtils::parseArgs(argc, argv, dirPath);

	

}

int MainUtils::parseArgs(int argc, char ** argv, std::string & dirPath)
{
	if (argc > 1 && strcmp(argv[1], "-threads"))
	{
		dirPath = argv[1];
	}

	auto iter = checkIfExists(argv, argv + argc, "-threads");
	if (iter)
	{
		++iter;
		try
		{
			auto numOfThreads = std::stoi(*iter);
			return numOfThreads;
		}
		catch (...) {}
	}
	return DEFAULT_THREAD_NUM;
}

char** MainUtils::checkIfExists(char ** begin, char ** end, const std::string & option)
{
	auto ret = begin;
	while (ret != end)
	{
		if (!option.compare(*ret))
			return ret;

		++ret;
	}
	return nullptr;
}