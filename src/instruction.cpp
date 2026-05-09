#include "instruction.h"
#include "encoder.h"
#include <iostream>

void rtype::encode(encoder& Encoder){
	Encoder.encode_rtype(_Inst, _Rd, _Rs1, _Rs2);
	return;
}
void itype::encode(encoder& Encoder){
	Encoder.encode_itype(_Inst, _Rd, _Rs, _Imm12b);
	return;
}
void stype::encode(encoder& Encoder){
	Encoder.encode_stype(_Inst, _Rs1, _Rs2, _Imm12b); 
	return;
}
void btype::encode(encoder& Encoder){
	Encoder.encode_btype(_Inst, _Rs1, _Rs2, _Imm12b);
	return;
}
void utype::encode(encoder& Encoder){
	Encoder.encode_utype(_Inst, _Rd, _Imm20b);
	return;
}
void jtype::encode(encoder& Encoder){
	Encoder.encode_jtype(_Inst, _Rd, _Imm20b);
	return;
}
