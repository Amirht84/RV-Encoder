#include "printer.h"
#include <iostream>
#include <bitset>

std::vector<std::string> printer::Paper;
void printer::add_line(const std::string& Line){
	Paper.push_back(Line);
}
void printer::flush(){
	for(auto& Line : Paper){
		std::cout << Line << '\n';
//		std::cout << "0x" << std::hex << std::bitset<32>(Line).to_ulong() << '\n';
	}
}
