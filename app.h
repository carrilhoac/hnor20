
#ifndef __C_HNOR2020_APP_H__
#define __C_HNOR2020_APP_H__

#include <string>
#include <vector>

#include "args.h"
#include "hnor20.h"

class CAppHnor {
private:
	struct HnorInput  {
		double 		d_lat;
		double 		d_lon;
	};
private:
	CArgParse 		args;
	HNOR 			geoid;
		
	std::vector<PointResult>  result;
	std::vector<HnorInput>  inputs;
	
public:
	CAppHnor(int argc, char **argv);
	
	void 			ParseLine(const std::string& s_line, int col_lat, int col_lon);
	bool 			ReadTextFile(const std::string& txt_file_name, int col_lat, int col_lon);
	void 			ExecSinglePoint(void);
			
	void 			ExecTextFile(void);
	bool 			DelegateTask(void);
	std::string 	Help(void);
};

#endif 