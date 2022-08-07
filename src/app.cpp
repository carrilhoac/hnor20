
#include <iostream>
#include <fstream>
#include <sstream>

#include "app.h"
#include "filesys.h"

CAppHnor::CAppHnor(int argc, char **argv)
	: args(argc, argv)
{ 
	DelegateTask();
}

void CAppHnor::WriteCSV(const std::string& s_name)const{
	std::ofstream out_file;
	
	out_file.open(s_name.c_str());
	out_file << PointResult::GetHeaderCSV() << std::endl;
	for (std::size_t i = 0; i < result.size(); ++i) {
		out_file << result[i].ToStringCSV() << std::endl;
	}
	
	out_file.close();
}
	
void CAppHnor::WriteTXT(const std::string& s_name)const{
	std::ofstream out_file;
	
	out_file.open(s_name.c_str());
	out_file << PointResult::GetHeaderTXT() << std::endl;
	for (std::size_t i = 0; i < result.size(); ++i) {
		out_file << result[i].ToStringTXT() << std::endl;
	}
	
	out_file.close();
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
	bool proc_exec = false;
	if (args.CheckOption("lat") && args.CheckOption("lon"))
	{
		ExecSinglePoint();
		proc_exec = true;
	}		
	
	if (args.CheckOption("in"))
	{
		ExecTextFile();
		proc_exec = true;
	}
	
	if (proc_exec)
	{
		if (args.CheckOption("out"))
		{
			std::string dst_fn = args.GetValueStr("out");
		
			if (filesys::check_extension(dst_fn.c_str(), "csv"))
				WriteCSV(dst_fn);
			else
				WriteTXT(dst_fn);
			
		}
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
	"Ferramenta desenvolvida por Andre Caceres Carrilho\n\n";

	s_help += 
	"UTILIZACAO:\n"
	"  1.   hnor lat=-15.8 lon=-47.865\n"
	"  2.   hnor lat=-15.8 lon=-47.865 out=resultado.txt\n"
	"  3.   hnor lon=-47.865 lat=-15.8 out=resultado.csv\n"
	"  4.   hnor in=pontos.txt out=resultado.csv\n"
	"  5.   hnor in=pontos.txt col_lat=2 col_lon=3 out=resultado.csv\n";
	
	return s_help;
}