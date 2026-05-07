#ifndef __INSTRUCTIONFACTORY__
#define __INSTRUCTIONFACTORY__

#include "type.h"

class instructionFactory {
	private:
		template<int B>
		bool is_overflow(const std::string&);
		bool is_num(const std::string&);
		bool is_dec(const std::string&);
		bool is_hex(const std::string&);
		bool is_offset_basereg_token(const std::string&);
		std::pair<std::string, std::string> seperated_offset_basereg_token(const std::string&);
		std::vector<std::string> seperate(std::string);
		bool is_format_r(const std::vector<std::string>&);
		bool is_format_i(const std::vector<std::string>&);
		bool is_format_s(const std::vector<std::string>&);
		bool is_format_b(const std::vector<std::string>&);
		bool is_format_u(const std::vector<std::string>&);
		bool is_format_j(const std::vector<std::string>&);
		void clean_line(std::string&);
		bool string_to_int(const std::string&);
	public:
		instruction* parse(std::string);
};

#endif
