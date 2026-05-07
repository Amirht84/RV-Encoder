#include "instruction.h"
#include "encoder.h"
#include <iostream>

void rtype::encode(encoder& Encoder){
	Encoder.encode_rtype(_inst, _rd, _rs1, _rs2);
	return;
}
void itype::encode(encoder& Encoder){
	Encoder.encode_itype(_inst, _rd, _rs, _imm12b);
	return;
}
void stype::encode(encoder& Encoder){
	Encoder.encode_stype(_inst, _rs1, _rs2, _imm12b); 
	return;
}
void btype::encode(encoder& Encoder){
	Encoder.encode_btype(_inst, _rs1, _rs2, _imm12b);
	return;
}
void utype::encode(encoder& Encoder){
	Encoder.encode_utype(_inst, _rd, _imm20b);
	return;
}
void jtype::encode(encoder& Encoder){
	Encoder.encode_jtype(_inst, _rd, _imm20b);
	return;
}
