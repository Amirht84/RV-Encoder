#include "configurator.h"
#include <stringstream>

static std::string omit_crlf(const std::string& input){
	if(!input.empty() && input.back() == '\r'){
		return input.substr(0, input.size() - 1);
	}
	return input;
}

static std::vector<std::string> parse_line(std::string line){
	std::vector<std::string> row;
	std::stringstream ss(line);
	std::string cell;
	while(std::getline(ss, cell, ',')){
		row.push_back(cell);
	}
	return row;
}
static bool is_opcode_table_header(const std::vector<std::string>& Header){
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
static bool is_register_table_header(const std::vector<std::string>& Header){
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
static type encoder::get_type(const std::string& Type){
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
static void configurator::configure_instruction(const std::string& fileName){
	std::ifstream file(fileName);
	std::string line;

	if(!file.is_open()){
		throw std::runtime_error("can't open file " + fileName);
	}

	getline(file, line);
	line = omit_crlf(line);
	auto Header = parse_line(line);
	if(!is_opcode_table_header(Header)){
		throw std::runtime_error("not a valid format for opcode_table");
	}
	while(getline(file, line)){
		line = omit_crlf(line);
		auto row = parse_line(line);// such Type, Ins, op, func3, func7
		for(int i = 0 ; i < 3 ; ++i){
			if(row[i] == ""){
				throw std::runtime_error("unexpected empty cell in instruction table");
			}
		}
		auto Type = get_type(row[0]);
		auto& inst = row[1];
		auto op = stoi(row[2]);
		auto func3 = (row[3] != "") ? stoi(row[3]) : -1;
		auto func7 = (row[4] != "") ? stoi(row[4]) : -1;
		InstMap.insert({inst, {op, func3, func7, Type}});
	}
	return;
}
static void configurator::configure_register(const std::string& fileName){
	std::ifstream file(fileName);
	std::string line;
	
	if(!file.is_open()){
		throw std::runtime_error("can't open file " + fileName);
	}

	getline(file, line);
	line = omit_crlf(line);
	auto Header = parse_line(line);
	if(!is_register_table_header(Header)){
		throw std::runtime_error("not a valid format for register_table");
	}
	while(getline(file, line)){
		line = omit_crlf(line);
		auto row = parse_line(line); //such Name, Number; i.g zero, x0
		if(row[0].empty() || row[1].empty()){
			throw std::runtime_error("unexpected empty cell in register_table");
		}
		if(row[1][0] != 'x'){
			throw std::runtime_error("not a vaild format for register numbers");
		}
		auto regName = row[0];
		auto regNumber = row[1];
		RegMap.insert({regName, regNumber});
	}
	return;
}


static std::array<int, 3> get_opcode(const std::string& inst) const {
	if(InstMap.find(inst) == InstMap.end()){
		throw std::runtime_error("syntax error or instruction not available in configuration file");
	}
	auto& [op, func3, func7, Type] = InstMap[inst];
	return {op, func3, func7};
}
static type get_type(const std::string inst) const {
	if(InstMap.find(inst) == InstMap.end()){
		throw std::runtime_error("syntax error or instruction not available in configuration file");
	}
	auto [op, func3, func7, Type] = InstMap[inst];
	return Type;
}
static std::string get_register_number(const std::string& reg) const {
	if(RegMap.find(reg) == RegMap.end()){
		throw std::runtime_error("syntax error, not a valid register name");
	}
	return RegMap[reg];
}
static bool is_register(const std::string& reg) const {
	if(reg[0] == 'x'){
		for(int i = 1; i < reg.size() ; ++i){
			if(!isdigit(reg[i])){
				return false;
			}
		}
		return true;
	}
	if(RegMap.find(reg) != RegMap.end()){
		return true;
	}
	return false;
}

