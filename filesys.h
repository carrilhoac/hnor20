
#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <algorithm>
#include <string>

///////////////////////////////////////////////////////////////////
//
// alternative to std::filesystem due to MinGW being out of date
//
///////////////////////////////////////////////////////////////////

namespace filesys 
{
	template<class T>
	T str_to_upper(const T& src)
	{
		T dst = src ;
		std::transform(dst.begin(), dst.end(), dst.begin(), ::toupper);
		return dst;	
	}
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
	template<class T>
	T get_extension(T const & filename)
	{
		return filename.substr( filename.find_last_of('.') +1);		
	}
	
	std::string 	str_to_upper(const char *src);
	std::string 	base_name(const char *path);
	std::string 	remove_extension(const char *filename);
	std::string 	get_extension(const char *filename);
	
//	bool 			check_extension();
}

#endif 