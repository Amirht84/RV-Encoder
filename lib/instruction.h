#ifndef __INSTRUCTION__
#define __INSTRUCTION__

#include <string>
#include "encoder.h"

class instruction {
	public:
		virtual void encode (encoder&) = 0;
		virtual ~instruction() = default;
};

class rtype : public instruction {
	protected:
		const std::string _inst;
		const int _Rd;
		const int _Rs1;
		const int _Rs2;
	public:
		rtype(
			const std::string& inst,
			const int Rd,
			const int Rs1,
			const int Rs2
		     ):
			_inst(inst), 
			_Rd(Rd),
			_Rs1(Rs1),
			_Rs2(Rs2) {}
		void encode (encoder&) override;
};

class itype : public instruction {
	private:
		const std::string _inst;
		const int _Rd;
		const int _rs;
		const int _Imm12b;
	public:
		itype(
			const std::string& inst,
			const int Rd,
			const int rs,
			const int Imm12b
		     ):
			_inst(inst),
			_Rd(Rd),
			_rs(rs),
			_Imm12b(Imm12b) {}
		void encode (encoder&) override;
};

class stype : public instruction {
	private:
		const std::string _inst;
		const int _Rs1;
		const int _Rs2;
		const int _Imm12b;
	public:
		stype(
			const std::string& inst,
			const int Rs1,
			const int Rs2,
			const int Imm12b
		     ):
			_inst(inst),
			_Rs1(Rs1),
			_Rs2(Rs2),
			_Imm12b(Imm12b){}
		void encode (encoder&) override;
};

class btype : public instruction {
	private:
		const std::string _inst;
		const int _Rs1;
		const int _Rs2;
		const int _Imm12b;
	public:
		btype(
			const std::string& inst,
			const int Rs1,
			const int Rs2,
			const int Imm12b
		     ):
			_inst(inst),
			_Rs1(Rs1),
			_Rs2(Rs2),
			_Imm12b(Imm12b) {}
		void encode (encoder&) override;
};

class utype : public instruction {
	private:
		const std::string _inst;
		const int _Rd;
		const int _Imm20b;
	public:
		utype(
			const std::string& inst,
			const int Rd,
			const int Imm20b
		     ):
			_inst(inst),
			_Rd(Rd),
			_Imm20b(Imm20b) {}
		void encode (encoder&) override;
};

class jtype : public instruction {
	private:
		const std::string _inst;
		const int _Rd;
		const int _Imm20b;
	public:
		jtype(
			const std::string& inst,
			const int Rd,
			const int Imm20b
		     ):
			_inst(inst),
			_Rd(Rd),
			_Imm20b(Imm20b) {}

		void encode (encoder&) override;
};
#endif
