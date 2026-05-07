#include "encoder.h"
#include "configurator.h"
#include "printer.h"
#include <bitset>
#include <iostream>

std::string encoder::shufler(const std::string& input, const std::vector<std::string>& codes){
	auto decode_bracket = [](const std::string& bracket) -> std::pair<int, int> {
		int firstNumber;
		int secondNumber;
		for(int i = 1 ; i < bracket.size() ; ++i){
			if(bracket[i] == ':'){
				firstNumber = std::stoi(bracket.substr(1, i - 1));
				secondNumber = std::stoi(bracket.substr(i + 1, bracket.size() - i - 2));
				break;
			}
		}
		return {firstNumber, secondNumber};
	};
	auto normalize_bit_index = [](int bitIndex, int size){
		return size - 1 - bitIndex;
	};
	
	std::string shufled = "";

	for(auto& code : codes){
		if(code[0] == '['){
			auto [firstNumber, secondNumber] = decode_bracket(code);
			int firstIndex = normalize_bit_index(firstNumber, input.size());
			shufled += input.substr(firstIndex, firstNumber - secondNumber + 1);
		}else{
			int index = normalize_bit_index(stoi(code), input.size());
			shufled += input[index];
		}
	}
	return shufled;
	
}

void encoder::encode_rtype(const std::string& inst, const int rd, const int rs1, const int rs2){
	auto RD  = std::bitset<5>(rd).to_string();
	auto RS1 = std::bitset<5>(rs1).to_string();
	auto RS2 = std::bitset<5>(rs2).to_string();
	auto CodeArray = configurator::get_opcode(inst); // contains op, func3, func7 in order
	auto OP = std::bitset<7>(CodeArray[0]).to_string();
	auto F3 = std::bitset<3>(CodeArray[1]).to_string();
	auto F7 = std::bitset<7>(CodeArray[2]).to_string();

	printer::add_line(F7 + RS2 + RS1 + F3 + RD + OP);
}
void encoder::encode_itype(const std::string& inst, const int rd, const int rs, const int imm12b){
	auto RD = std::bitset<5>(rd).to_string();
	auto RS = std::bitset<5>(rs).to_string();
	auto IMM12B = std::bitset<12>(imm12b).to_string();
	auto CodeArray = configurator::get_opcode(inst);
	auto OP = std::bitset<7>(CodeArray[0]).to_string();
	auto F3 = std::bitset<3>(CodeArray[1]).to_string();

	printer::add_line( IMM12B + RS + F3 + RD + OP);

}
void encoder::encode_stype(const std::string& inst, const int rs1, const int rs2, const int imm12b){
	auto RS1 = std::bitset<5>(rs1).to_string();
	auto RS2 = std::bitset<5>(rs2).to_string();
	auto IMM12B = std::bitset<12>(imm12b).to_string();
	auto CodeArray = configurator::get_opcode(inst);
	auto OP = std::bitset<7>(CodeArray[0]).to_string();
	auto F3 = std::bitset<3>(CodeArray[1]).to_string();

	printer::add_line( shufler(IMM12B, {"[11:5]"})+ RS2+ RS1+ F3+ shufler(IMM12B, {"[4:0]"})+ OP);
}
void encoder::encode_btype(const std::string& inst, const int rs1, const int rs2, const int imm12b){
	auto RS1 = std::bitset<5>(rs1).to_string();
	auto RS2 = std::bitset<5>(rs2).to_string();
	auto IMM12B = std::bitset<13>(imm12b).to_string();
	auto CodeArray = configurator::get_opcode(inst);
	auto OP = std::bitset<7>(CodeArray[0]).to_string();
	auto F3 = std::bitset<3>(CodeArray[1]).to_string();

	printer::add_line( shufler(IMM12B, {"12", "[10:5]"})+ RS2+ RS1+ F3+ shufler(IMM12B, {"[4:1]", "11"})+ OP);
}
void encoder::encode_utype(const std::string& inst, const int rd, const int imm20b){
	auto RD = std::bitset<5>(rd).to_string();
	auto IMM20B = std::bitset<20>(imm20b).to_string();
	auto CodeArray = configurator::get_opcode(inst);
	auto OP = std::bitset<7>(CodeArray[0]).to_string();

	printer::add_line(IMM20B + RD + OP);
}
void encoder::encode_jtype(const std::string& inst, const int rd, const int imm20b){
	auto RD = std::bitset<5>(rd).to_string();
	auto IMM20B = std::bitset<21>(imm20b).to_string();
	auto CodeArray = configurator::get_opcode(inst);
	auto OP = std::bitset<7>(CodeArray[0]).to_string();

	printer::add_line(shufler(IMM20B, {"20", "[10:1]", "11", "[19:12]"}) + RD + OP);
}

