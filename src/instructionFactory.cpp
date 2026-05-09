#include "instructionFactory.h"
#include <vector>
#include <string>
#include <stdexcept>

void instructionFactory::clean_line(std::string& Line){
	std::replace(Line.begin(), Line.end(), '\t', ' ');
	std::replace(Line.begin(), Line.end(), ',', ' ');
}

std::pair<std::string, std::string> seperate_offset_basereg_token(const std::string& Token){
	// token is offset_basereg by default
	size_t StartBase = Token.find('(');
	std::string Offset = Token.substr(0, StartBase);
	std::string BaseReg = Token.substr(StartBase + 1, Token.size() - StartBase - 2);
	return {BaseReg, Offset};
}

std::vector<std::string> instructionFactory::tokenate(std::string Line){
	std::vector<std::string> Tokens;
	std::string Token;
	
	clean_line(Line);

	std::stringstream LineStream(Line);
	while (LineStream >> Token) {
		if(!Token.empty()){
			if(is_offset_basereg_token(Token)){
				auto [BaseReg, Offset] = seperate_offset_basereg_token(Token);
				Tokens.push_back(BaseReg);
				Tokens.push_back(Offset);
			}else{
				Tokens.push_back( Token );
			}
		}
	}
	if(Tokens.size() > 4) throw std::runtime_error("each instruction must contain at most 4 tokens");
	return Tokens;
}

bool instructionFactory::is_hex(const std::string& Input){
	size_t StartInd = (Input[0] == '-') ? 1 : 0;
	if(Input.substr(StartInd, 2) == "0x" || Input.substr(0, 2) == "0X"){
		if(StartInd + 2 == Input.size()){
			return false;
		}
		for(size_t i = StartInd + 2 ; i < Input.size() ; ++i){
			if(!isxdigit(Input[i])){
				return false;
			}
		}
		return true;
	}
	return false;
}

bool instructionFactory::is_dec(const std::string& Input){
	int StartInd = (Input[0] == '-') ? 1 : 0;
	for(size_t i = StartInd ; i < Input.size() ; ++i){
		if(!isdigit(Input[i])){
			return false;
		}
	}
	return true;

}
bool instructionFactory::is_num(const std::string& Input){
	if(is_hex(Input) || is_dec(Input)){
		return true;
	}
	return false;

}

template<int B>
bool instructionFactory::is_overflow(const std::string& Input){
	int Num = string_to_int(Input);
	auto StringedBit = std::bitset<B + 1>(Num).to_string();
	return StringedBit[0] != StringedBit[1];
}

bool instructionFactory::is_offset_basereg_token(const std::string& Token){
	size_t StartBasePos = Token.find('(');
	size_t EndBasePos = Token.find(')');
	if( StartBasePos == std::string::npos || EndBasePos == std::string::npos ){
		return false;
	}
	if( StartBasePos == EndBasePos || EndBasePos != Token.size() - 1){
		return false;
	}
	if( StartBasePos == 0 ){
		return false;
	}
	return true;

}
bool instructionFactory::is_format_r(const std::vector<std::string>& Tokens){
	if(Tokens.size() != 4){
		return false;
	}
	for(int i = 1 ; i < 4; ++i){
		if(!configurator::is_reg(Tokens[i])) return false;
	}
	return true;
}
bool instructionFactory::is_format_i(const std::vector<std::string>& Tokens){
	if(Tokens.size() != 4){
		return false;
	}
	for(int i = 1 ; i < 3 ; ++i){
		if(!configurator::is_reg(Tokens[i])) return false;
	}
	if(!is_num(Tokens[3])){
		return false;
	}
	if(is_overflow<12>(Tokens[3])){
		return false;
	}
	return true;
}
bool instructionFactory::is_format_s(const std::vector<std::string>& Tokens){
	if(Tokens.size() != 4){
		return false;
	}
	for(int i = 1 ; i < 3 ; ++i){
		if(!configurator::is_reg(Tokens[i])){
			return false;
		}
	}
	if(!is_num(Tokens[3])){
		return false;
	}
	if(is_overflow<12>(Tokens[3])){
		return false;
	}
	return true;
}
bool instructionFactory::is_format_b(const std::vector<std::string>& Tokens){
	if(Tokens.size() != 4){
		return false;
	}
	for(int i = 1 ;i < 3 ; ++i){
		if(!configurator::is_reg(Tokens[i])){
			return false;
		}
	}
	if(!is_num(Tokens[3])){
		return false;
	}
	if(is_overflow<13>(Tokens[3])){
		return false;
	}
	return true;
}
bool instructionFactory::is_format_u(const std::vector<std::string>& Tokens){
	if(Tokens.size() != 3){
		return false;
	}
	if(!configurator::is_reg(Tokens[1])){
		return false;
	}
	if(!is_num(Tokens[2])){
		return false;
	}
	if(is_overflow<20>(Tokens[2])){
		return false;
	}
	return true;
}
bool instructionFactory::is_format_j(const std::vector<std::string>& Tokens){
	if(Tokens.size() != 3){
		return false;
	}
	if(!configurator::is_reg(Tokens[1])){
		return false;
	}
	if(!is_num(Tokens[2])){
		return false;
	}
	if(is_overflow<21>(Tokens[2])){
		return false;
	}
	return true;
}

int instructionFactory::regtag_to_int(std::string Regtag){
	if(Regtag[0] != 'x'){
		Regtag = configurator::get_register_number(Regtag);
	}
	return std::stoi(Regtag.substr(1, Regtag.size() - 1));
}
int instructionFactory::string_to_int(const std::string& Input){
	int StartInd = (Input[0] == '-') ? 1 : 0;
	int Num = ( 1 - 2 * StartInd ) * std::stoi(Input.substr(StartInd, Input.size() - StartInd), nullptr, 0);
	return Num;
}
instruction* instructionFactory::parse(std::string Line){
	auto Tokens = tokenate(Line);
	if(Tokens.size() != 3 && Tokens.size() != 4){
		throw std::runtime_error("syntax error not valid emounts of tokens for instruction");
	}

	instruction* ParsedInst = nullptr;
	std::string& InstToken = Tokens[0];
	auto Type = configurator::get_instruction_type(InstToken);

	switch(Type){
		case R:
		{
			if(!is_format_r(Tokens)){
				throw std::runtime_error("not a true format for an R-Type Instruction");
			}
			int Rd = regtag_to_int(Tokens[1]);
			int Rs1 = regtag_to_int(Tokens[2]);
			int Rs2 = regtag_to_int(Tokens[3]);
			ParsedInst = new rtype(InstToken, Rd,  Rs1, Rs2);
			break;
		}
		case I:
		{
			if(!is_format_i(Tokens)){
				throw std::runtime_error("not a true format for an I-Type Instruction");
			}
			int Rd = regtag_to_int(Tokens[1]);
			int Rs = regtag_to_int(Tokens[2]);
			int Imm12b = string_to_int(Tokens[3]);
			ParsedInst = new itype(InstToken, Rd, Rs, Imm12b);
			break;
		}
		case S:
		{
			if(!is_format_s(Tokens)){
				throw std::runtime_error("not a true format for an S-Type Instruction");
			}
			int Rs1 = regtag_to_int(Tokens[2]);
			int Rs2 = regtag_to_int(Tokens[1]);
			int Imm12b = string_to_int(Tokens[3]);
			ParsedInst = new stype(InstToken, Rs1, Rs2, Imm12b);
			break;
		}
		case B:
		{
			if(!is_format_b(Tokens)){
				throw std::runtime_error("not a true format for a B-Type Instruction");
			}
			int Rs1 = regtag_to_int(Tokens[1]);
			int Rs2 = regtag_to_int(Tokens[2]);
			int Imm12b = string_to_int(Tokens[3]);
			ParsedInst = new btype(InstToken, Rs1, Rs2, Imm12b);
			break;
		}
		case U:
		{
			if(!is_format_u(Tokens)){
				throw std::runtime_error("not a true format for a U-Type Instruction");
			}
			int Rd = regtag_to_int(Tokens[1]);
			int Imm20b = string_to_int(Tokens[2]);
			ParsedInst = new utype(InstToken, Rd, Imm20b);
			break;
		}
		case J:
		{
			if(!is_format_j(Tokens)){
				throw std::runtime_error("not a true format for a J-Type Instruction");
			}
			int Rd = regtag_to_int(Tokens[1]);
			int Imm20b = string_to_int(Tokens[2]);
			ParsedInst = new jtype(InstToken, Rd, Imm20b);
			break;
		}
	}
	if(ParsedInst == nullptr){
		//logic error must be chekced in the next version
	}
	return ParsedInst;
}
