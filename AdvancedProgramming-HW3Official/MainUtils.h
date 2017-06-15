#pragma once
#include <string>

class MainUtils
{
public:
	static const int DEFAULT_THREAD_NUM = 4;
	static const char COMMENT_SIGN = '#';
	static const char EQUALS = '=';

	static const std::string DEFAULT_LOGFILE_NAME;
	static const std::string CONFIG_END;
	static const std::string BOARD_END;
	static const std::string DLL_END;
	static const std::string WORKING_DIR;
	static const std::string SEPERATOR;
	static const std::string ASTERISK;
	static const std::string THREADS;
	static const std::string LOGGER_NAME;

	static int parseArgs(int argc, char** argv, std::string& dirPath);
	static char ** checkIfExists(char ** begin, char ** end, const std::string & option);
	static std::string getDefaultParamsFromConfig(int& numOfThreads, std::ifstream& config); 
};
