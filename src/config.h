#ifndef config_H
#define config_H

#include <string>
#include <map>

#define COMMENT_CHAR		('#')
#define DEFAULT_FILE		("default.cfg")
#define CFG_PATH			("./cfg/")

enum seedType{
	SEED_STATIC,
	SEED_TIME_BASED
};

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