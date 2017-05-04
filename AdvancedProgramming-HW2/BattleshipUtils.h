#pragma once

#include <string>
#include <iostream>

enum class Direction
{
	NON,
	VERTICAL,
	HORIZONTAL,
};

class BattleshipUtils
{
public:
	/**
	 * Parses a line (std::string) to a pair of indices.
	 * 
	 * @param str - the line to parse
	 * @param coord - to be assigned with indices
	 * 
	 * @return true - if line is valid, false - o.w.
	 */
	static bool parseLineAndValidate(const std::string& str, std::pair<int, int>& coord);

	/**
	 *
	 *TODO update doc.
	 *
	* Checks if filename has a suitable extension (.attack-a/.attack-b/.sboard),
	* if it does - assigns to files in the matching index.
	*
	* @param files - an array of strings to be assigned with input files (if exist)
	* @param filename - a candidte to be an input file
	*/
	static bool endsWith(const std::string& filename, const std::string& suffix);

	static char** allocateBoard(size_t rows, size_t cols);

	static void	deallocateBoard(char** board, size_t rows);

};



