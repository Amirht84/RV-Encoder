#ifndef __INSTRUCTIONFACTORY__
#define __INSTRUCTIONFACTORY__

#include "type.h"
#include "instruction.h"
#include "configurator.h"
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <sstream>
#include <bitset>

class instructionFactory {
	private:
		template<int B>
		bool is_overflow(const std::string&);
		bool is_num(const std::string&);
		bool is_dec(const std::string&);
		bool is_hex(const std::string&);
		bool is_offset_basereg_token(const std::string&);
		std::pair<std::string, std::string> seperated_offset_basereg_token(const std::string&);
		std::vector<std::string> tokenate(std::string);
		bool is_format_r(const std::vector<std::string>&);
		bool is_format_i(const std::vector<std::string>&);
		bool is_format_s(const std::vector<std::string>&);
		bool is_format_b(const std::vector<std::string>&);
		bool is_format_u(const std::vector<std::string>&);
		bool is_format_j(const std::vector<std::string>&);
		void clean_line(std::string&);
		int string_to_int(const std::string&);
		int regtag_to_int(std::string);
	public:
		instruction* parse(std::string);
};

#endif
