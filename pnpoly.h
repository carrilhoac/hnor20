
#ifndef __IBGE_PNPOLY_H__
#define __IBGE_PNPOLY_H__


class PnPoly{
private:

	// SANTANA
	double p_sant_lon[ 8] = { -49.76,-49.76,-50.50,-52.00,-52.99,-56.20,-56.20,-49.76 };
	double p_sant_lat[ 8] = {   4.74,  1.00, -0.48, -1.49, -1.70,  1.00,  4.74,  4.74 };
	
	// Mapgeo2015
	double p_mgeo_lon[ 7] = { -64.5,-64.5,-70.0,-70.0,-74.2,-74.2,-64.5 };
	double p_mgeo_lat[ 7] = {   3.0, -2.0, -8.0,-11.4,-11.4,  3.0,  3.0 };

public:
	bool	InsideSANTANA(double g_lat, double g_lon) const;
	bool	InsideMAPGEO15(double g_lat, double g_lon) const;
	
public:
	static PnPoly& Get() 
	{
		static PnPoly 	instance;
		return instance;
	}
private:
	PnPoly() {} 

public:
	PnPoly(const PnPoly& ) = delete;
	void operator =(const PnPoly& ) = delete;
};

#endif 