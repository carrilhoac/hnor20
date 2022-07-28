
#include "hnor20.h"
#include "utm.h"
#include "pnpoly.h"

#include <iostream>


int main(int argc, char **argv)
{
   // TestInterp();
	HNOR geoid;
	
	std::cout << geoid.Get(-20.414, -49.980);
	std::cout << geoid.Get( -0.027, -51.194);
	std::cout << geoid.Get( -4.317, -68.388);
	
	return 0;
}
