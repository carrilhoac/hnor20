
#include "hnor20.h"
#include "utm.h"
#include "pnpoly.h"
#include "args.h"

#include <iostream>

class CAppHnor {
private:
	CArgParse 	args;
	HNOR 		geoid;
		
public:
	CAppHnor(int argc, char **argv);
	
	void ExecSinglePoint(void)
	{
		std::cout << args.GetValueDbl("lat") << std::endl;
		std::cout << args.GetValueDbl("lon") << std::endl;
	}
	
	bool DelegateTask(void)
	{
		if (args.CheckOption("lat") && args.CheckOption("lon"))
		{
			ExecSinglePoint();
			return true;
		}		
		
		std::cout << Help();
		return false;
	}
	
	static std::string Help(void)
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
};

CAppHnor::CAppHnor(int argc, char **argv)
	: args(argc, argv)
{ 
	DelegateTask();
}

int main(int argc, char **argv)
{
	CAppHnor  m_app(argc, argv);
	
	
	#if 0
	HNOR geoid;
	
	std::cout << geoid.Get(-20.414, -49.980);
	std::cout << geoid.Get( -0.027, -51.194);
	std::cout << geoid.Get( -4.317, -68.388);
	#endif 
	
	return 0;
}
