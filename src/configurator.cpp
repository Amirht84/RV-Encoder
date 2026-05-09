#include "configurator.h"
#include <sstream>
#include <fstream>

std::map<std::string, instructionInfo> configurator::InstMap;
std::map<std::string, std::string> configurator::RegMap;

void configurator::skip_bom(std::ifstream& File){
	if(!File.is_open()){
		return;
	}

	std::streampos OriginalPos = File.tellg();

	char Bom[3];
	File.read(Bom, 3);
	bool HasBom = false;
	if(File.gcount() == 3){
		HasBom = (static_cast<unsigned char>(Bom[0]) == 0xEF)
			&& (static_cast<unsigned char>(Bom[1]) == 0xBB)
			&& (static_cast<unsigned char>(Bom[2]) == 0xBF);
	}
	if(!HasBom){
		File.clear();
		File.seekg(OriginalPos);
	}
	return;
}

std::string configurator::omit_crlf(const std::string& Input){
	if(!Input.empty() && Input.back() == '\r'){
		return Input.substr(0, Input.size() - 1);
	}
	return Input;
}

std::vector<std::string> configurator::parse_line(std::string Line){
	std::vector<std::string> Row;
	std::stringstream RowStream(Line);
	std::string Cell;
	while(std::getline(RowStream, Cell, ',')){
		Row.push_back(Cell);
	}
	return Row;
}
bool configurator::is_opcode_table_header(const std::vector<std::string>& Header){
	if(Header.size() != 5){
		return false;
	}
	if(Header[0] != "Type"){
		return false;
	}
	if(Header[1] != "Ins"){
		return false;
	}
	if(Header[2] != "op"){
		return false;
	}
	if(Header[3] != "func3"){
		return false;
	}
	if(Header[4] != "func7"){
		return false;
	}
	return true;
}
bool configurator::is_register_table_header(const std::vector<std::string>& Header){
	if(Header.size() != 2){
		return false;
	}
	if(Header[0] != "Name"){
		return false;
	}
	if(Header[1] != "Number"){
		return false;
	}
	return true;
}
type configurator::get_type(const std::string& Type){
	if(Type == "R"){
		return R;
	}
	if(Type == "I"){
		return I;
	}
	if(Type == "S"){
		return S;
	}
	if(Type == "B"){
		return B;
	}
	if(Type == "U"){
		return U;
	}
	if(Type == "J"){
		return J;
	}
	throw std::runtime_error("unknown type in instruction table");
}
void configurator::configure_instruction(const std::string& FileName){
	std::ifstream File(FileName, std::ios::binary);
	std::string Line;

	if(!File.is_open()){
		throw std::runtime_error("can't open File " + FileName);
	}
	skip_bom(File);


	getline(File, Line);
	Line = omit_crlf(Line);
	auto Header = parse_line(Line);

	if(!is_opcode_table_header(Header)){
		throw std::runtime_error("not a valid format for opcode_table");
	}

	while(getline(File, Line)){
		Line = omit_crlf(Line);
		auto Row = parse_line(Line);// such Type, Ins, op, func3, func7

		if(Row.size() < 3){
			throw std::runtime_error("unexpected empty cell in instruction table");
		}

		auto Type = get_type(Row[0]);
		auto& Ints = Row[1];
		auto Op = stoi(Row[2]);
		auto Func3 = (Row.size() < 4 || Row[3].empty()) ? -1 : stoi(Row[3]);
		auto Func7 = (Row.size() < 5 || Row[4].empty()) ? -1 : stoi(Row[4]);
		InstMap.insert({Ints, {Op, Func3, Func7, Type}});
	}
	return;
}
void configurator::configure_register(const std::string& FileName){
	std::ifstream File(FileName, std::ios::binary);
	std::string Line;
	
	if(!File.is_open()){
		throw std::runtime_error("can't open File " + FileName);
	}
	skip_bom(File);

	getline(File, Line);
	Line = omit_crlf(Line);
	auto Header = parse_line(Line);
	if(!is_register_table_header(Header)){
		throw std::runtime_error("not a valid format for register_table");
	}
	while(getline(File, Line)){
		Line = omit_crlf(Line);
		auto Row = parse_line(Line); //such Name, Number; i.g zero, x0
		if(Row.size() < 2){
			throw std::runtime_error("unexpected empty cell in register_table");
		}
		if(Row[1][0] != 'x'){
			throw std::runtime_error("not a vaild format for register numbers");
		}
		auto RegName = Row[0];
		auto RegNumber = Row[1];
		RegMap.insert({RegName, RegNumber});
	}
	return;
}


std::array<int, 3> configurator::get_opcode(const std::string& Ints) {
	if(InstMap.find(Ints) == InstMap.end()){
		throw std::runtime_error("syntax error or instruction not available in configuration File");
	}
	auto& [Op, Func3, Func7, _] = InstMap[Ints];
	return {Op, Func3, Func7};
}
type configurator::get_instruction_type(const std::string& Ints) {
	if(InstMap.find(Ints) == InstMap.end()){
		throw std::runtime_error("syntax error or instruction not available in configuration File");
	}
	auto [_, _, _, Type] = InstMap[Ints];
	return Type;
}
std::string configurator::get_register_number(const std::string& reg) {
	if(RegMap.find(reg) == RegMap.end()){
		throw std::runtime_error("syntax error, not a valid register name");
	}
	return RegMap[reg];
}
bool configurator::is_reg(const std::string& Reg) {
	if(reg[0] == 'x'){
		for(int i = 1; i < Reg.size() ; ++i){
			if(!isdigit(Reg[i])){
				return false;
			}
		}
		return true;
	}
	if(RegMap.find(Reg) != RegMap.end()){
		return true;
	}
	return false;
}
