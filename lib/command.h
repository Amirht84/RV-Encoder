#ifndef COMMAND
#define COMMAND

class instruction {
	public:
		virtual void encode () = 0;
		virtual ~instruction() = default;
};

class rtype : public instruction {
	protected:
		const int _rd;
		const int _rs1;
		const int _rs2;
		const int _op;
		const int _func3;
		const int _func7;
	public:
		rtype(
			const int rd,
			const int rs1,
			const int rs2,
			const int op,
			const int func3,
			const int func7
			):
			_rd(rd),
			_rs1(rs1),
			_rs2(rs2),
			_op(op),
			_func3(func3),
			_func7(func7) {}
};

class addC : public rtype {
	public:
		addC(
			const int rd,
			const int rs1,
			const int rs2
			):
			rtype(rd, rs1, rs2, 51, 0, 0) {}
	private:
		void encode() override;
};

class subC : public rtype {
	public:
		subC(
			const int rd,
			const int rs1,
			const int rs2
			):
			rtype(rd, rs1, rs2, 51, 0, 32) {}
	private:
		void encode() override;
};
class andC : public rtype {
	public:
		andC(
			const int rd,
			const int rs1,
			const int rs2
			):
			rtype(rd, rs1, rs2, 51, 7, 0) {}
	private:
		void encode() override;
};
class orC : public rtype {
	public:
		orC(
			const int rd,
			const int rs1,
			const int rs2
			):
			rtype(rd, rs1, rs2, 51, 6, 0) {}
	private:
		void encode() override;
};



class sltC : public rtype {
	public:
		sltC(
			const int rd,
			const int rs1,
			const int rs2,
			):
			rtype(rd, rs1, rs2, 51, 2, 0) {}
	private:
		void encode() override;
};

class lwC : public instruction {
	private:		
	public:
		void excecute() override;

};


class addiC : public instruction {
	private:
	public:
		void excecute() override;
}

class xoriC : public instruction {
	private:
		
	public:
		void excecute() override;

};

class oriC : public instruction {
	private:
		
	public:
		void excecute() override;

};


class sltiC : public instruction {
	private:
		
	public:
		void excecute() override;

};


class jalrC : public instruction {
	private:
		
	public:
		void excecute() override;

};


class swC : public instruction {
	private:
		
	public:
		void excecute() override;

};


class jalC : public instruction {
	private:
		
	public:
		void excecute() override;

};

class beqC : public instruction {
	private:
		
	public:
		void excecute() override;

};


class bneC : public instruction {
	private:
		
	public:
		void excecute() override;

};


class bltC : public instruction {
	private:
		
	public:
		void excecute() override;

};


class bgeC : public instruction {
	private:
		
	public:
		void excecute() override;

};

#endif
