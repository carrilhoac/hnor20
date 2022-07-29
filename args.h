
#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

//#include <iostream>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////
class CArgParse {
private:
	//static std::string 		_StrToUpper(const std::string& src);

private:
	std::vector<std::string> 		args;
	std::vector<std::string> 		args_upper;
	
public:
	CArgParse(int argc, char **argv);
	
	int				Count(void)const { return int(args.size()); }
	
	bool 			CheckOption(const std::string& s_opt)const;
	std::string 	GetValueStr(const std::string& s_opt, const std::string& s_sep = "=")const;
	int 			GetValueInt(const std::string& s_opt, const std::string& s_sep = "=")const;
	double 			GetValueDbl(const std::string& s_opt, const std::string& s_sep = "=")const;
};

#endif 
