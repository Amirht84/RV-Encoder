#include "instructionFactory.h"
#include "encoder.h"
#include "configurator.h"
#include "instruction.h"
#include <iostream>

int main(){
	instructionFactory InstructionFactory;
	encoder Encoder;
	configurator.configure_register("configuration/registers_table.csv");
	configurator.configure_instruction("configuration/opcode_table.csv");
	std::string line;
	while(getline(std::cin, line)){
		if(line[line.size() - 1] == '\r'){
			line.erase(line.size() - 1, 1);
		}
		auto inst = InstructionFactory.parse(line);
		inst->encode(Encoder);
	}
}
