#include "encoder.h"
#include <bitset>

static void encoder::add_configure(const std::string& inst, const int op, const int func3, const int func7){
	_InstructionHolder.insert({inst, {op, func3, func7}});
}

template<int bits>
std::string encoder::get_stringed_bit(const int Num){
	std::bitset<bits> B(Num);
	return B.to_string();

}

void encoder::encode_rtype(const std::string& inst, const int rd, const int rs1, const int rs2){
	auto RD  = std::bitset<5>(rd).to_string();
	auto RS1 = std::bitset<5>(rs1).to_string();
	auto RS2 = std::bitset<5>(rs2).to_string();
	auto CodeArray = InstructionHolder[inst]; //contains op, func3, func7 in order
	auto OP = std::bitset<7>(CodeArray[0]).to_string();
	auto F3 = std::bitset<3>(CodeArray[1]).to_string();
	auto F7 = std::bitset<7>(CodeArray[2]).to_string();

	std::cout << F7 + RS2 + RS1 + F3 + RD + OP << '\n';

}


