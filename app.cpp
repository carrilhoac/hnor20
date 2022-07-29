
#include <iostream>
#include <fstream>
#include <sstream>

#include "app.h"

CAppHnor::CAppHnor(int argc, char **argv)
	: args(argc, argv)
{ 
	DelegateTask();
}

void CAppHnor::ParseLine(const std::string& s_line, int col_lat, int col_lon)
{		
	std::stringstream ss(s_line);
	std::vector<std::string> s_tokens;
	std::string s_piece;
	
	while (ss.good())
	{
		ss >> s_piece;
		s_tokens.push_back( s_piece );
	}
	
	HnorInput c_point;
	c_point.d_lat = std::stod( s_tokens[col_lat] );
	c_point.d_lon = std::stod( s_tokens[col_lon] );
	inputs.push_back( c_point );
	
	result.push_back( geoid.Get(c_point.d_lat, c_point.d_lon) );
//	std::cout << c_point.d_lat << " " << c_point.d_lon << std::endl;
}

bool CAppHnor::ReadTextFile(const std::string& txt_file_name, int col_lat, int col_lon)
{
	std::ifstream in_file;
	in_file.open(txt_file_name.c_str());
	
	while (in_file.good())
	{
		std::string s_curline;
		std::getline(in_file, s_curline);
					
		ParseLine(s_curline, col_lat, col_lon);
	}
	
	in_file.close();
	return true;
}

void CAppHnor::ExecSinglePoint(void)
{
	double lat = args.GetValueDbl("lat");
	double lon = args.GetValueDbl("lon");
	
	result.push_back( geoid.Get(lat, lon) );
	std::cout << result.back() << std::endl;
}

void CAppHnor::ExecTextFile(void)
{
	int n_col_lat = 0;
	int n_col_lon = 1;
			
	if (args.CheckOption("col_lat"))
		n_col_lat = args.GetValueInt("col_lat");
	if (args.CheckOption("col_lon"))
		n_col_lon = args.GetValueInt("col_lon");
			
	ReadTextFile( args.GetValueStr("in"), n_col_lat, n_col_lon );
}

bool CAppHnor::DelegateTask(void)
{
	if (args.CheckOption("lat") && args.CheckOption("lon"))
	{
		ExecSinglePoint();
		return true;
	}		
	
	if (args.CheckOption("in"))
	{
		ExecTextFile();
		return true;
	}
	
	std::cout << Help();
	return false;
}

std::string CAppHnor::Help(void)
{
	std::string s_help;
	
	s_help = 
	"\n\tHNOR20\n\n"
	"Aplicativo para obtencao da separacao (ondulacao) geoidal, isto e diferenca entre altitudes\n"
	"geometricas e altitudes ortometricas (fisicas) utilizando o modelo geoidal hgeoHNOR20202,\n"
	"disponibilzado pelo Instituto Brasileiro de Geografia e Estatistica - IBGE\n\n"
	"Ferramenta desenvolvida por Andre Caceres Carrilho\n";
	
	return s_help;
}