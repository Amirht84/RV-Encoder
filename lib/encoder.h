#ifndef ENCODER
#define ENCODER

#include <array>
#include <string>

class encoder {
	private:
		std::string& get_stringed_bit(const int);
		static std::map<std::string, std::array<int, 3>> _InstructionHolder;
	public:
		static void add_configure(const std::string&, const int, const int func3 = -1, const int func7 = -1);
		void encode_rtype(const std::string&, const int, const int, const int);
		void encode_itype(const std::string&, const int, const int, const int);
		void encode_stype(const std::string&, const int, const int, const int);
		void encode_btype(const std::string&, const int, const int, const int);
		void encode_utype(const std::string&, const int, const int);
		void encode_jtype(const std::string&, const int);
};

#endif
