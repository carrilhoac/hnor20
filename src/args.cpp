#include "args.h"
#include "filesys.h"

#include <iostream>

CArgParse::CArgParse(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		args.push_back(argv[i]);
		
		if (args.back()[0] == '-')
			args.back().erase(0,1);
		
		args_upper.push_back( str_to_upper(args.back()) );
	}
}
bool CArgParse::CheckOption(const std::string& s_opt)const
{
	if (!args.size())
		return false;
	
	std::size_t n_opt = s_opt.length();
	std::string s_opt_upper = str_to_upper(s_opt);
	
	for (std::size_t i = 0; i < args_upper.size(); ++i)
		if (args_upper[i].compare(0, n_opt, s_opt_upper) == 0)
			return true;
	
	return false;
}
std::string CArgParse::GetValueStr(const std::string& s_opt, const std::string& s_sep)const
{
	if (!args.size())
		return "";
	
	std::string s_opt_upper = str_to_upper(s_opt);
	
	for (std::size_t i = 0; i < args_upper.size(); ++i)
	{
		std::size_t s_opt_len = s_opt.length();
		std::string s_arg = args_upper[i].substr(0, s_opt_len);
		
		if (s_opt_upper.compare(s_arg) == 0)
		{
			std::size_t n_sep = args[i].find_last_of(s_sep) +1; 
			return args[i].substr(n_sep);
		}
	}
	
	return "";
}
int CArgParse::GetValueInt(const std::string& s_opt, const std::string& s_sep)const
{
	return std::stoi( GetValueStr(s_opt, s_sep) );
}
double CArgParse::GetValueDbl(const std::string& s_opt, const std::string& s_sep)const
{
	return std::stod( GetValueStr(s_opt, s_sep) );
}