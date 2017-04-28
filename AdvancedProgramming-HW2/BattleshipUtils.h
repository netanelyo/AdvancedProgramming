#pragma once

#include <string>
#include <iostream>


class BattleshipConstants
{
public:
	const static size_t RUBBER_BOAT_POINTS	= 2;
	const static size_t MISSILE_BOAT_POINTS = 3;
	const static size_t SUBMARINE_POINTS	= 7;
	const static size_t DESTROYER_POINTS	= 8;

	const static size_t RUBBER_BOAT_LEN		= 1;
	const static size_t MISSILE_BOAT_LEN	= 2;
	const static size_t SUBMARINE_LEN		= 3;
	const static size_t DESTROYER_LEN		= 4;
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

	static char** allocateBoard(int rows, int cols);

	static void	deallocate(char** board, int rows);
	
};



