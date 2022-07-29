
#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

//#include <iostream>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////
//
// alternative to STD::FILESYSTEM due to:
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=78870
//
///////////////////////////////////////////////////////////////////
namespace PATH 
{
    template<class T>
    T base_name(T const & path, T const & delims = "/\\")
    {
      return path.substr(path.find_last_of(delims) + 1);
    }
    template<class T>
    T remove_extension(T const & filename)
    {
      typename T::size_type const p(filename.find_last_of('.'));
      return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
    }
}

///////////////////////////////////////////////////////////////////
class CArgParse {
private:
	static std::string 		_StrToUpper(const std::string& src);

private:
	std::vector<std::string> 		args;
	std::vector<std::string> 		args_upper;
	
public:
	CArgParse(int argc, char **argv);
	
	bool 			CheckOption(const std::string& s_opt)const;
	std::string 	GetValueStr(const std::string& s_opt)const;
	int 			GetValueInt(const std::string& s_opt)const;
	double 			GetValueDbl(const std::string& s_opt)const;
};

#endif 
