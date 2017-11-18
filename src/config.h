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
#define DEFAULT_FILE		("./cfg/default.cfg")


/**********************************************************
*	Types, Etc.
**********************************************************/
enum seedType {
	SEED_STATIC,
	SEED_TIME_BASED
};

enum parentSelType {
	PARENT_FP,
	PARENT_OS
};

enum survivalSelType {
	SURVIVAL_TRUNCATION,
	SURVIVAL_KTOURN
};

enum testTestType {
	TERMTEST_NUM_EVALS,
	TERMTEST_BEST_UNCHANGED	
};

enum dataType {
	INT,
	FLOAT,
	STRING
};

struct cfg_value {
	void* address;
	dataType type;
};


/**********************************************************
*	Config Class
**********************************************************/
struct config {

	std::string	srcfile	= DEFAULT_FILE;
	int			seed;
	int			seedType;
	int			mu;
	int			lambda;
	int			depth;
	int			memory;
	int			rounds;
	int			runs;
	int			evals;
	float		parsimony;
	float		mutation;
	int			rerandmem;
	int			parentSelection;
	int			survivalSelection;
	int			survivalK;
	int			termTest;
	int			unchanged;
	std::string	logfile;
	std::string	solutionfile;

	std::map<std::string, cfg_value> defs = {
		{ "seed", cfg_value{ &seed, INT } },
		{ "seedType", cfg_value{ &seedType, INT } },
		{ "mu", cfg_value{ &mu, INT } },
		{ "lambda", cfg_value{ &lambda, INT } },
		{ "l", cfg_value{ &rounds, INT } },
		{ "d", cfg_value{ &depth, INT } },
		{ "k", cfg_value{ &memory, INT } },
		{ "runs", cfg_value{ &runs, INT } },
		{ "evals", cfg_value{ &evals, INT } },
		{ "p", cfg_value{ &parsimony, FLOAT } },
		{ "mutation", cfg_value{ &mutation, FLOAT } },
		{ "rerandmem", cfg_value{ &rerandmem, INT } },
		{ "parentSelection", cfg_value{ &parentSelection, INT } },
		{ "survivalSelection", cfg_value{ &survivalSelection, INT } },
		{ "survivalK", cfg_value{ &survivalK, INT } },
		{ "unchanged", cfg_value{ &unchanged, INT } },
		{ "termTest", cfg_value{ &termTest, INT } },
		{ "logfile", cfg_value{ &logfile, STRING } },
		{ "solutionfile", cfg_value{ &solutionfile, STRING } }
	};

	// Constructors & Destructors
	inline config() { read(DEFAULT_FILE); };
	inline config(std::string filename) { read(filename); };

	// Member Functions
	bool read(std::string filename);

};


#endif
