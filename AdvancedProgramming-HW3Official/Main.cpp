#include "MainUtils.h"
#include "TournamentManager.h"
#include <windows.h>
#include <iostream>
#include <sstream>

#define PRINT_WRONG_PATH(path) std::cout << "Wrong path: " << path
#define PRINT_MISSING_BOARD_FILE(path) std::cout << \
								"No board files (*.sboard) looking in path: " << path
#define PRINT_MISSING_DLL_FILE(path) std::cout << \
								"Missing algorithm (dll) files looking in path: " << \
								path << " (needs at least two)"


const std::string MainUtils::DEFAULT_LOGFILE_NAME = "game.log";
const std::string MainUtils::CONFIG_END = ".config";
const std::string MainUtils::BOARD_END = ".sboard";
const std::string MainUtils::DLL_END = ".dll";
const std::string MainUtils::WORKING_DIR = ".";
const std::string MainUtils::SEPERATOR = "\\";
const std::string MainUtils::ASTERISK = "*";
const std::string MainUtils::THREADS = "threads";
const std::string MainUtils::LOGGER_NAME = "loggerName";
const std::string MainUtils::LOGGER_LEVEL = "loggerLevel";
const std::string MainUtils::ERROR_STRING = "ERROR";


int main(int argc, char** argv)
{
	std::string dirPath(MainUtils::WORKING_DIR);
	char cFullPath[MAX_PATH];
	WIN32_FIND_DATAA fileData;
	auto logName = MainUtils::DEFAULT_LOGFILE_NAME;
	auto level = LoggerLevel::WARNING;

	auto numOfThreads = MainUtils::parseArgs(argc, argv, dirPath);
	std::string fullPath = _fullpath(cFullPath, dirPath.c_str(), MAX_PATH);

	auto dir = FindFirstFileA((fullPath +
							MainUtils::SEPERATOR +
							MainUtils::ASTERISK +
							MainUtils::CONFIG_END).c_str(), &fileData);
	if (dir == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() != ERROR_FILE_NOT_FOUND)
		{
			PRINT_WRONG_PATH(fullPath);
			return EXIT_FAILURE;
		}
		if (numOfThreads == -1)
		{
			numOfThreads = MainUtils::DEFAULT_THREAD_NUM; 
		}
	}
	else 
	{
		std::ifstream config(fullPath + MainUtils::SEPERATOR + fileData.cFileName);
		MainUtils::getDefaultParamsFromConfig(numOfThreads, config, logName, level); 
		config.close();
	}

	TournamentManager tournament(numOfThreads, fullPath + MainUtils::SEPERATOR + logName, level);

	std::vector<std::string> boards;
	auto error = false;
	dir = FindFirstFileA((fullPath +
						MainUtils::SEPERATOR +
						MainUtils::ASTERISK +
						MainUtils::BOARD_END).c_str(), &fileData);
	if (dir == INVALID_HANDLE_VALUE)
	{
		PRINT_MISSING_BOARD_FILE(fullPath);
		error = true;
	}
	else
	{
		do
		{
			boards.push_back(fileData.cFileName);
		} while (FindNextFileA(dir, &fileData));
	}

	if (!error && !tournament.initializeBoards(boards, fullPath + MainUtils::SEPERATOR))
	{
		PRINT_MISSING_BOARD_FILE(fullPath);
		error = true;
	}

	std::vector<std::string> dlls;
	dir = FindFirstFileA((fullPath +
						MainUtils::SEPERATOR +
						MainUtils::ASTERISK +
						MainUtils::DLL_END).c_str(), &fileData);
	if (dir != INVALID_HANDLE_VALUE)
	{
		do
		{
			dlls.push_back(fileData.cFileName);
		} while (FindNextFileA(dir, &fileData));
	}
	if (dir == INVALID_HANDLE_VALUE || dlls.size() < 2 ||
		!tournament.initializeDlls(dlls, fullPath + MainUtils::SEPERATOR))
	{
		if (error)
			std::cout << std::endl;
		PRINT_MISSING_DLL_FILE(fullPath);
		return EXIT_FAILURE;
	}

	if (error)
		return EXIT_FAILURE;

	tournament.startTournament();
}

int MainUtils::parseArgs(int argc, char ** argv, std::string & dirPath)
{
	std::string th = "-" + THREADS;
	if (argc > 1 && th.compare(argv[1]))
	{
		dirPath = argv[1];
	}

	auto iter = checkIfExists(argv, argv + argc, th);
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
	return -1;
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

void MainUtils::getDefaultParamsFromConfig(int& numOfThreads, std::ifstream& config, std::string& logName, LoggerLevel& level)
{
	if (!config.is_open())
	{
		if (numOfThreads == -1)
		{
			numOfThreads = DEFAULT_THREAD_NUM;
		}
	}
	else
	{
		std::string line; 
		while (std::getline(config, line))
		{
			if (line[0] == COMMENT_SIGN)
				continue;
		
			std::string linePart;
			std::vector<std::string> lineParts;
			auto stream = std::istringstream(line);
			while (std::getline(stream, linePart, EQUALS))
			{
				lineParts.push_back(linePart);
			}

			if (lineParts.size() != 2)
				continue;

			if (numOfThreads == -1 && !lineParts[0].compare(THREADS))
			{
				try
				{
					numOfThreads = std::stoi(lineParts[1]);
				}
				catch (...)
				{
					numOfThreads = DEFAULT_THREAD_NUM;
				}
			}
			else if (!lineParts[0].compare(LOGGER_NAME))
				logName = lineParts[1];
			else if (!lineParts[0].compare(LOGGER_LEVEL))
				if (!lineParts[1].compare(ERROR_STRING))
					level = LoggerLevel::ERR;
		}	
	}
}