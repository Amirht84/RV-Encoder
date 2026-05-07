#include "instructionFactory.h"
#include <vector>
#include <string>
#include <stdexcept>

void instructionFactory::clean_line(std::string& line){
	std::replace(line.begin(), line.end(), '\t', ' ');
	std::replace(line.begin(), line.end(), ',', ' ');
}

std::pair<std::string, std::string> seperate_offset_basereg_token(const std::string& token){
	// token is offset_basereg by default
	size_t startBase = token.find('(');
	std::string offset = token.substr(0, startBase);
	std::string baseReg = token.substr(startBase + 1, token.size() - startBase - 2);
	return {baseReg, offset};
}
#include <iostream>
std::vector<std::string> instructionFactory::tokenate(std::string line){
	std::vector<std::string> tokens;
	std::string token;
	
	clean_line(line);

	std::stringstream ss(line);
	while (ss >> token) {
		if(!token.empty()){
			if(is_offset_basereg_token(token)){
				auto [baseReg, offset] = seperate_offset_basereg_token(token);
				tokens.push_back(baseReg);
				tokens.push_back(offset);
			}else{
				tokens.push_back( token );
			}
		}
	}
	for(int i = 0 ; i < tokens.size() ; ++i){
		std::cerr << tokens[i] << "|";
	}
	std::cerr << '\n';
	if(tokens.size() > 4) throw std::runtime_error("each instruction must contain at most 4 tokens");
	return tokens;
}

bool instructionFactory::is_hex(const std::string& input){
	int startInd = (input[0] == '-') ? 1 : 0;
	if(input.substr(startInd, 2) == "0x" || input.substr(0, 2) == "0X"){
		if(startInd + 2 == input.size()){
			return false;
		}
		for(int i = startInd + 2 ; i < input.size() ; ++i){
			if(!isxdigit(input[i])){
				return false;
			}
		}
		return true;
	}
	return false;
}

bool instructionFactory::is_dec(const std::string& input){
	int startInd = (input[0] == '-') ? 1 : 0;
	for(int i = startInd ; i < input.size() ; ++i){
		if(!isdigit(input[i])){
			return false;
		}
	}
	return true;

}
bool instructionFactory::is_num(const std::string& input){
	if(is_hex(input) || is_dec(input)){
		return true;
	}
	return false;

}

template<int B>
bool instructionFactory::is_overflow(const std::string& input){
	int num = string_to_int(input);
	auto stringedBit = std::bitset<B + 1>(num).to_string();
	return stringedBit[0] != stringedBit[1];
}

bool instructionFactory::is_offset_basereg_token(const std::string& token){
	size_t startBasePos = token.find('(');
	size_t endBasePos = token.find(')');
	if( startBasePos == std::string::npos || endBasePos == std::string::npos ){
		return false;
	}
	if( startBasePos == endBasePos || endBasePos != token.size() - 1){
		return false;
	}
	if( startBasePos == 0 ){
		return false;
	}
	return true;

}
bool instructionFactory::is_format_r(const std::vector<std::string>& tokens){
	if(tokens.size() != 4){
		return false;
	}
	for(int i = 1 ; i < 4; ++i){
		if(!configurator::is_reg(tokens[i])) return false;
	}
	return true;
}
bool instructionFactory::is_format_i(const std::vector<std::string>& tokens){
	if(tokens.size() != 4){
		return false;
	}
	for(int i = 1 ; i < 3 ; ++i){
		if(!configurator::is_reg(tokens[i])) return false;
	}
	if(!is_num(tokens[3])){
		return false;
	}
	if(is_overflow<12>(tokens[3])){
		return false;
	}
	return true;
}
bool instructionFactory::is_format_s(const std::vector<std::string>& tokens){
	if(tokens.size() != 4){
		return false;
	}
	for(int i = 1 ; i < 3 ; ++i){
		if(!configurator::is_reg(tokens[i])){
			return false;
		}
	}
	if(!is_num(tokens[3])){
		return false;
	}
	if(is_overflow<12>(tokens[3])){
		return false;
	}
	return true;
}
bool instructionFactory::is_format_b(const std::vector<std::string>& tokens){
	if(tokens.size() != 4){
		return false;
	}
	for(int i = 1 ;i < 3 ; ++i){
		if(!configurator::is_reg(tokens[i])){
			return false;
		}
	}
	if(!is_num(tokens[3])){
		return false;
	}
	if(is_overflow<13>(tokens[3])){
		return false;
	}
	return true;
}
bool instructionFactory::is_format_u(const std::vector<std::string>& tokens){
	if(tokens.size() != 3){
		return false;
	}
	if(!configurator::is_reg(tokens[1])){
		return false;
	}
	if(!is_num(tokens[2])){
		return false;
	}
	if(is_overflow<20>(tokens[2])){
		return false;
	}
	return true;
}
bool instructionFactory::is_format_j(const std::vector<std::string>& tokens){
	if(tokens.size() != 3){
		return false;
	}
	if(!configurator::is_reg(tokens[1])){
		return false;
	}
	if(!is_num(tokens[2])){
		return false;
	}
	if(is_overflow<21>(tokens[2])){
		return false;
	}
	return true;
}

int instructionFactory::regtag_to_int(std::string regtag){
	if(regtag[0] != 'x'){
		regtag = configurator::get_register_number(regtag);
	}
	return std::stoi(regtag.substr(1, regtag.size() - 1));
}
int instructionFactory::string_to_int(const std::string& input){
	int startInd = (input[0] == '-') ? 1 : 0;
	int num = ( 1 - 2 * startInd ) * std::stoi(input.substr(startInd, input.size() - startInd), nullptr, 0);
	return num;
}
instruction* instructionFactory::parse(std::string line){
	auto tokens = tokenate(line);
	if(tokens.size() != 3 && tokens.size() != 4){
		throw std::runtime_error("syntax error not valid emounts of tokens for instruction");
	}

	instruction* parsedInst = nullptr;
	std::string& instToken = tokens[0];
	auto Type = configurator::get_instruction_type(instToken);

	switch(Type){
		case R:
		{
			if(!is_format_r(tokens)){
				throw std::runtime_error("not a true format for an R-Type Instruction");
			}
			int rd = regtag_to_int(tokens[1]);
			int rs1 = regtag_to_int(tokens[2]);
			int rs2 = regtag_to_int(tokens[3]);
			parsedInst = new rtype(instToken, rd,  rs1, rs2);
			break;
		}
		case I:
		{
			if(!is_format_i(tokens)){
				throw std::runtime_error("not a true format for an I-Type Instruction");
			}
			int rd = regtag_to_int(tokens[1]);
			int rs = regtag_to_int(tokens[2]);
			int imm12b = string_to_int(tokens[3]);
			parsedInst = new itype(instToken, rd, rs, imm12b);
			break;
		}
		case S:
		{
			if(!is_format_s(tokens)){
				throw std::runtime_error("not a true format for an S-Type Instruction");
			}
			int rs1 = regtag_to_int(tokens[2]);
			int rs2 = regtag_to_int(tokens[1]);
			int imm12b = string_to_int(tokens[3]);
			parsedInst = new stype(instToken, rs1, rs2, imm12b);
			break;
		}
		case B:
		{
			if(!is_format_b(tokens)){
				throw std::runtime_error("not a true format for a B-Type Instruction");
			}
			int rs1 = regtag_to_int(tokens[1]);
			int rs2 = regtag_to_int(tokens[2]);
			int imm12b = string_to_int(tokens[3]);
			parsedInst = new btype(instToken, rs1, rs2, imm12b);
			break;
		}
		case U:
		{
			if(!is_format_u(tokens)){
				throw std::runtime_error("not a true format for a U-Type Instruction");
			}
			int rd = regtag_to_int(tokens[1]);
			int imm20b = string_to_int(tokens[2]);
			parsedInst = new utype(instToken, rd, imm20b);
			break;
		}
		case J:
		{
			if(!is_format_j(tokens)){
				throw std::runtime_error("not a true format for a J-Type Instruction");
			}
			int rd = regtag_to_int(tokens[1]);
			int imm20b = string_to_int(tokens[2]);
			parsedInst = new jtype(instToken, rd, imm20b);
			break;
		}
	}
	if(parsedInst == nullptr){
		//logic error must be chekced in the next version
	}
	return parsedInst;
}
