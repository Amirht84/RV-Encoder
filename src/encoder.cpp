#include "encoder.h"
#include "configurator.h"
#include "printer.h"
#include <bitset>
#include <iostream>

std::string encoder::shufler(const std::string& Input, const std::vector<std::string>& Codes){
	auto decode_bracket = [](const std::string& Bracket) -> std::pair<int, int> {
		int FirstNumber;
		int SecondNumber;
		for(size_t i = 1 ; i < Bracket.size() ; ++i){
			if(Bracket[i] == ':'){
				FirstNumber = std::stoi(Bracket.substr(1, i - 1));
				SecondNumber = std::stoi(Bracket.substr(i + 1, Bracket.size() - i - 2));
				break;
			}
		}
		return {FirstNumber, SecondNumber};
	};
	auto normalize_bit_index = [](int BitIndex, int Size){
		return Size - 1 - BitIndex;
	};
	
	std::string Shufled = "";

	for(auto& Code : Codes){
		if(Code[0] == '['){
			auto [FirstNumber, SecondNumber] = decode_bracket(Code);
			int FirstIndex = normalize_bit_index(FirstNumber, Input.size());
			Shufled += Input.substr(FirstIndex, FirstNumber - SecondNumber + 1);
		}else{
			int index = normalize_bit_index(stoi(Code), Input.size());
			Shufled += Input[index];
		}
	}
	return Shufled;
	
}

void encoder::encode_rtype(const std::string& Inst, const int Rd, const int Rs1, const int Rs2){
	auto RdStr  = std::bitset<5>(Rd).to_string();
	auto Rs1Str = std::bitset<5>(Rs1).to_string();
	auto Rs2Str = std::bitset<5>(Rs2).to_string();
	auto CodeArray = configurator::get_opcode(Inst); // contains op, func3, func7 in order
	auto Op = std::bitset<7>(CodeArray[0]).to_string();
	auto Func3 = std::bitset<3>(CodeArray[1]).to_string();
	auto Func7 = std::bitset<7>(CodeArray[2]).to_string();

	printer::add_line(Func7 + Rs2Str + Rs1Str + Func3 + RdStr + Op);
}
void encoder::encode_itype(const std::string& Inst, const int Rd, const int Rs, const int Imm12b){
	auto RdStr = std::bitset<5>(Rd).to_string();
	auto RS = std::bitset<5>(Rs).to_string();
	auto Imm12bStr = std::bitset<12>(Imm12b).to_string();
	auto CodeArray = configurator::get_opcode(Inst);
	auto Op = std::bitset<7>(CodeArray[0]).to_string();
	auto Func3 = std::bitset<3>(CodeArray[1]).to_string();

	printer::add_line( Imm12bStr + RS + Func3 + RdStr + Op);

}
void encoder::encode_stype(const std::string& Inst, const int Rs1, const int Rs2, const int Imm12b){
	auto Rs1Str = std::bitset<5>(Rs1).to_string();
	auto Rs2Str = std::bitset<5>(Rs2).to_string();
	auto Imm12bStr = std::bitset<12>(Imm12b).to_string();
	auto CodeArray = configurator::get_opcode(Inst);
	auto Op = std::bitset<7>(CodeArray[0]).to_string();
	auto Func3 = std::bitset<3>(CodeArray[1]).to_string();

	printer::add_line( shufler(Imm12bStr, {"[11:5]"})+ Rs2Str+ Rs1Str+ Func3+ shufler(Imm12bStr, {"[4:0]"})+ Op);
}
void encoder::encode_btype(const std::string& Inst, const int Rs1, const int Rs2, const int Imm12b){
	auto Rs1Str = std::bitset<5>(Rs1).to_string();
	auto Rs2Str = std::bitset<5>(Rs2).to_string();
	auto Imm12bStr = std::bitset<13>(Imm12b).to_string();
	auto CodeArray = configurator::get_opcode(Inst);
	auto Op = std::bitset<7>(CodeArray[0]).to_string();
	auto Func3 = std::bitset<3>(CodeArray[1]).to_string();

	printer::add_line( shufler(Imm12bStr, {"12", "[10:5]"})+ Rs2Str+ Rs1Str+ Func3+ shufler(Imm12bStr, {"[4:1]", "11"})+ Op);
}
void encoder::encode_utype(const std::string& Inst, const int Rd, const int Imm20b){
	auto RdStr = std::bitset<5>(Rd).to_string();
	auto Imm20bStr = std::bitset<20>(Imm20b).to_string();
	auto CodeArray = configurator::get_opcode(Inst);
	auto Op = std::bitset<7>(CodeArray[0]).to_string();

	printer::add_line(Imm20bStr + RdStr + Op);
}
void encoder::encode_jtype(const std::string& Inst, const int Rd, const int Imm20b){
	auto RdStr = std::bitset<5>(Rd).to_string();
	auto Imm20bStr = std::bitset<21>(Imm20b).to_string();
	auto CodeArray = configurator::get_opcode(Inst);
	auto Op = std::bitset<7>(CodeArray[0]).to_string();

	printer::add_line(shufler(Imm20bStr, {"20", "[10:1]", "11", "[19:12]"}) + RdStr + Op);
}

