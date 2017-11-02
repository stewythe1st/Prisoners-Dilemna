/******************************************************************************
*	config.h
*
*	Declarations for the config class for the Iterated Prisoner's Dilemna Problem
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/
#ifndef config_H
#define config_H


/**********************************************************
*	Headers
**********************************************************/
#include <string>
#include <map>


/**********************************************************
*	Compiler Constants
**********************************************************/
#define COMMENT_CHAR		('#')
#define DEFAULT_FILE		("default.cfg")
#define CFG_PATH			("./cfg/")


/**********************************************************
*	Types, Etc.
**********************************************************/
enum seedType{
	SEED_STATIC,
	SEED_TIME_BASED
};


/**********************************************************
*	Config Class
**********************************************************/
struct config {

	std::string	srcfile	= DEFAULT_FILE;
	int			seed;
	int			seedType;
	int			depth;
	int			memory;

	std::map<std::string, int*>	defs = {
		{"seed", &seed},
		{"seedType", &seedType},
		{"d", &depth},
		{"k", &memory }
	};

	// Constructors & Destructors
	inline config() { read(DEFAULT_FILE); };
	inline config(std::string filename) { read(filename); };

	// Member Functions
	bool read(std::string filename);

};


#endif
