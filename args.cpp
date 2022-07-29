#include "args.h"
#include <algorithm>

std::string CArgParse::_StrToUpper(const std::string& src)
{
	std::string dst = src ;
	std::transform(dst.begin(), dst.end(), dst.begin(), ::toupper);
	return dst;	
}

CArgParse::CArgParse(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		args.push_back(argv[i]);
		
		if (args.back()[0] == '-')
			args.back().erase(0,1);
		
		args_upper.push_back( _StrToUpper(args.back()) );
	}
}
bool CArgParse::CheckOption(const std::string& s_opt)const
{
	std::string s_opt_upper = _StrToUpper(s_opt);
	
	for (std::size_t i = 0; i < args_upper.size(); ++i)
		if (s_opt_upper.compare(args_upper[i]) == 0)
			return true;
	
	return false;
}
std::string CArgParse::GetValueStr(const std::string& s_opt)const
{
	std::string s_opt_upper = _StrToUpper(s_opt);
	
	for (std::size_t i = 0; i < args_upper.size(); ++i)
	{
		std::size_t s_opt_len = s_opt.length();
		std::string s_arg = args_upper[i].substr(0, s_opt_len);
		
		if (s_opt_upper.compare(s_arg) == 0)
		{
			std::size_t s_sep = args[i].find_last_of("=") +1; 
			return args[i].substr(s_sep);
		}
	}
	
	return "";
}
int CArgParse::GetValueInt(const std::string& s_opt)const
{
	return std::stoi( GetValueStr(s_opt) );
}
double CArgParse::GetValueDbl(const std::string& s_opt)const
{
	return std::stod( GetValueStr(s_opt) );
}