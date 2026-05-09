#include "instructionFactory.h"
#include "encoder.h"
#include "configurator.h"
#include "instruction.h"
#include <iostream>
#include "printer.h"
int main(){
	instructionFactory InstructionFactory;
	encoder Encoder;
	configurator::configure_register("./configuration/registers_table.csv");
	configurator::configure_instruction("./configuration/opcode_table.csv");
	std::string Line;
	while(getline(std::cin, Line)){
		if(Line[Line.size() - 1] == '\r'){
			Line.erase(Line.size() - 1, 1);
		}
		auto inst = InstructionFactory.parse(Line);
		inst->encode(Encoder);
	}
	printer::flush();
}
