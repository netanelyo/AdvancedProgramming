#pragma once
#include <fstream>


enum class LoggerLevel
{
	ERROR,
	WARNING,
	//DEBUG //TODO: redundant?
};

class Logger
{
public:
	class LoggerMessage
	{
	public:
		static const std::string PROGRAM_START;
		static const std::string WARNING_UNBALANCED_BOARD;
		static const std::string ERROR_CANT_OPEN_BOARD_FILE;
		static const std::string ERROR_WRONG_DIMENSIONS_LINE;
		static const std::string ERROR_ADJACENT_SHIPS;
		static const std::string ERROR_INVALID_SHAPE_OR_SIZE;
		static const std::string ERROR_CANT_OPEN_DLL;
		static const std::string ERROR_CANT_LOAD_DLL;

		friend class Logger;
		
	private:
		static const std::string ERROR;
		static const std::string WARNING;
	};

	Logger(const std::string& loggerFilePath, LoggerLevel level = LoggerLevel::WARNING);
	~Logger() { if (m_isOpen) m_logger.close(); }

	void printToLogger(const std::string& msg, LoggerLevel level);

private:
	LoggerLevel		m_level;
	std::ofstream	m_logger;
	bool			m_isOpen;

	static std::string getTimeAndDate();
	void printMessage(const std::string& msg, bool first = false);
};