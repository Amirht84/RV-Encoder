#ifndef __PRINTER__
#define __PRINTER__

class printer{
	private:
		static std::vector<std::string> paper;
	public:
		static void add_line(const std::string&);
		static void flush();
};

#endif
