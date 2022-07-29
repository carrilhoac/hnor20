
#include "filesys.h"

namespace filesys 
{
	std::string str_to_upper(const char *src) 
		{ 
		std::string str = src;
		return str_to_upper(str); 
		}
		
	std::string base_name(const char *path)
		{ 
		std::string str = path;
		return base_name(str);
		}
		
	std::string remove_extension(const char *filename)
		{ 
		std::string str = filename;
		return remove_extension(str);
		}
	
	std::string get_extension(const char *filename)
		{ 
		std::string str = filename;
		return get_extension(str);
		}
		
	bool check_extension(const char *filename, const char *extension)
	{
		std::string s_ext = str_to_upper(get_extension(filename));
		std::string s_cmp = str_to_upper(extension);
		return s_ext.compare(s_cmp) == 0;
	}
}

