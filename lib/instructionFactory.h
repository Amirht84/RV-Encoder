#ifndef __INSTRUCTIONFACTORY__
#define __INSTRUCTIONFACTORY__

#include "type.h"

class instructionFactory {
	private:
		std::map<std::string, type> TypeMap;
		std::array<std::string, 4> seperate(std::string);
	public:
		void add_configure(const std::string&, const type);
		instruction* parse(std::string);
};

#endif
