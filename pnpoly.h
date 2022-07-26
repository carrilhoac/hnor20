
#ifndef __IBGE_PNPOLY_H__
#define __IBGE_PNPOLY_H__

class PolySANTANA{
private:
	double poly_lon[ 8] = { -49.76,-49.76,-50.50,-52.00,-52.99,-56.20,-56.20,-49.76 };
	double poly_lat[ 8] = {   4.74,  1.00, -0.48, -1.49, -1.70,  1.00,  4.74,  4.74 };

public:
	bool	Inside(double g_lat, double g_lon) const;
	
public:
	static PolySANTANA& Get() 
	{
		static PolySANTANA 	instance;
		return instance;
	}
private:
	PolySANTANA() {} 

public:
	PolySANTANA(const PolySANTANA& ) = delete;
	void operator =(const PolySANTANA& ) = delete;
};

#endif 