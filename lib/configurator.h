#ifndef __CONFIGURATOR__
#define __CONFIGURATOR__
#include <vector>
#include <string>
#include "type.h"

struct instructionInfo {
	int op;
	int func3;
	int func7;
	type Type;
};

class configurator {
	private:
		static std::map<std::string, instructionInfo> InstMap;
		static std::map<std::string, std::string> RegMap;
		static std::string omit_crlf(const std::string&);
		static std::vector<std::string> parse_line(std::string);
		static bool is_opcode_table_header(const std::vector<std::string>&);
		static bool is_register_table_header(const std::vector<std::string>&);
		static type get_type(const std::string&);
	public:
		static void configure_instruction(const std::string&);
		static void configure_register(const std::string&);
		static std::array<int, 3> get_opcode(const std::string&) const ;
		static type get_instruction_type(const std::string&) const ;
		static std::string get_register_number(const std::string&)const ;
		static bool is_register(const std::string&) const ;
};

#endif
