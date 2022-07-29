
#include "app.h"

#include "filesys.h"
#include "args.h"

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
