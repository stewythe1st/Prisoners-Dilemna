/******************************************************************************
*	config.cpp
*
*	Definitions for the config class for the Iterated Prisoner's Dilemna Problem
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/


/**********************************************************
*	Headers
**********************************************************/
#include "config.h"
#include <fstream>


/**********************************************************
*	config::read()
*	Reads in a configuration from the passed filename.
*	Expects file to be in th CFG_PATH directory and to be
*	in the form of "token=value". Any line starting with 
*	a COMMENT_CHAR will be ignored.
*	 @param filename name of the file to be read
**********************************************************/
bool config::read(std::string filename) {

	// Variables
	std::ifstream	in;
	std::string		line;
	std::size_t		split;
	std::string		lhs;
	std::string		rhs;
	std::map<std::string, cfg_value>::iterator
					value;

	// Open file
	srcfile = filename;
	in.open(filename);
	if (!in.is_open()) {
		return false;
	}

	// Read in line-by-line
	while (getline(in, line)) {

		// Skip comment lines
		if (line[0] == COMMENT_CHAR)
			continue;

		// Handling for Windows-style line endings
		if (line.length() > 1 && line[line.length() - 1] == '\r')
			line.pop_back();

		// Split line into token(lhs) and value(rhs)
		split = line.find("=");
		if (split != std::string::npos) {
			lhs = line.substr(0, split);
			rhs = line.substr(split + 1);
		}
		
		value = defs.find(lhs);
		if (value != defs.end()) {
			if ((*value).second.type == INT)
				*(int*)(*value).second.address = atoi(rhs.c_str());
			else if ((*value).second.type == STRING)
				*(std::string*)(*value).second.address = rhs;
		}

		
	}

	// Clean up
	in.close();
	return true;
}
