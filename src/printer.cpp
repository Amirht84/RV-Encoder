#include "printer.h"
#include <iostream>

void printer::add_line(const std::string& Line){
	paper.push_back(Line);
}
void printer::flush(){
	for(auto& Line : paper){
		std::cout << Line << '\n';
	}
}
