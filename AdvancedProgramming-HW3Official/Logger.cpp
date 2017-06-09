#include "Logger.h"
#include <ctime>
#include <string>


const std::string Logger::LoggerMessage::PROGRAM_START = "Battleship application started";
const std::string Logger::LoggerMessage::WARNING_UNBALANCED_BOARD = "Ship types between the players are not balanced - ";
const std::string Logger::LoggerMessage::ERROR_CANT_OPEN_BOARD_FILE = "Board file couldn't be opened - ";
const std::string Logger::LoggerMessage::ERROR_WRONG_DIMENSIONS_LINE = "Wrong board file format - ";
const std::string Logger::LoggerMessage::ERROR_ADJACENT_SHIPS = "Adjacent ships on board - ";
const std::string Logger::LoggerMessage::ERROR_INVALID_SHAPE_OR_SIZE = "Invalid shape or size of ship on board - ";
const std::string Logger::LoggerMessage::ERROR_CANT_OPEN_DLL = "Dll file couldn't be opened - ";
const std::string Logger::LoggerMessage::ERROR_CANT_LOAD_DLL = "Dll file couldn't be loaded - ";
const std::string Logger::LoggerMessage::ERROR = "ERROR: ";
const std::string Logger::LoggerMessage::WARNING = "WARNING: ";

Logger::Logger(const std::string & loggerFilePath, LoggerLevel level) :
	m_level(level),
	m_logger(loggerFilePath, std::ios::trunc | std::ios::out),
	m_isOpen(m_logger.is_open())
{
	printMessage(LoggerMessage::PROGRAM_START, true);
}

void Logger::printToLogger(const std::string& msg, LoggerLevel level)
{
	switch (level)
	{
	case LoggerLevel::ERROR:
		printMessage(LoggerMessage::ERROR + msg);
		break;

	case LoggerLevel::WARNING:
		if (m_level == LoggerLevel::WARNING)
			printMessage(LoggerMessage::WARNING + msg);

	default:
		break;
	}
}

std::string Logger::getTimeAndDate()
{
	// Thanks to SOF
	auto rawtime = time(nullptr);
	char buffer[100];
	struct tm timeinfo;

	time(&rawtime);
	auto ret = localtime_s(&timeinfo, &rawtime);
	if (ret)
		return "";

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &timeinfo);

	return std::string(buffer);
}

void Logger::printMessage(const std::string & msg, bool first)
{
	auto timeAndDate = getTimeAndDate();

	if (!first)
		m_logger << std::endl;

	if (m_isOpen)
		m_logger << timeAndDate << " " << msg;
}
