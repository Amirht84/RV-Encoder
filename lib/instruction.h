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
		const int _rd;
		const int _rs1;
		const int _rs2;
	public:
		rtype(
			const std::string& inst,
			const int rd,
			const int rs1,
			const int rs2
		     ):
			_inst(inst), 
			_rd(rd),
			_rs1(rs1),
			_rs2(rs2) {}
		void encode (encoder&) override;
};

class itype : public instruction {
	private:
		const std::string _inst;
		const int _rd;
		const int _rs;
		const int _imm12b;
	public:
		itype(
			const std::string& inst,
			const int rd,
			const int rs,
			const int imm12b
		     ):
			_isnt(inst),
			_rd(rd),
			_rs(rs),
			_imm12b(imm12b) {}
		void encode (encoder&) override;
};

class stype : public instruction {
	private:
		const std::string _inst;
		const int _rs1;
		const int _rs2;
		const int _imm12b;
	public:
		stype(
			const std::strign& inst,
			const int rs1,
			const int rs2,
			const int _imm12b
		     ):
			_inst(inst),
			_rs1(rs1),
			_rs2(rs2),
			_imm12b(imm12b) {}
		void encode (encoder&) override;
};

class btype : public instruction {
	private:
		const std::string _inst;
		const int _rs1;
		const int _rs2;
		const int _imm12b;
	public:
		btype(
			const std::string& inst,
			const int rs1,
			const int rs2,
			const int imm12b
		     ):
			_inst(inst),
			_rs1(rs1),
			_rs2(rs2),
			_imm12b(imm12b) {}
		void encode (encoder&) override;
};

class utype : public instruction {
	private:
		const std::string _inst;
		const int _rd;
		const int _imm20b;
	public:
		utype(
			const std::string& inst,
			const int rd,
			const int imm20b
		     ):
			_inst(inst),
			_rd(rd),
			_imm20b(imm20b) {}
		void encode (encoder&) override;
};

class jtype : public instruction {
	private:
		const std::string _inst;
		const int _rd;
		const int _imm20b;
	public:
		utype(
			const std::string& inst,
			const int rd,
			const int imm20b
		     ):
			_inst(inst),
			_rd(rd),
			_imm20b(imm20) {}

		void encode (encoder&) override;
};
#endif
