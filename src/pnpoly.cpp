
#include "pnpoly.h"

#if __cplusplus < 199711L
	#error "this code requires C++11 or later"
#endif 

extern "C" 
{
	// Source: https://wrfranklin.org/Research/Short_Notes/pnpoly.html
	// Returns 1 if inside 0 otherwise
	static int pnpoly(int nvert, const double *vertx, const double *verty, double testx, double testy)
	{
	  int i, j, c = 0;
	  for (i = 0, j = nvert-1; i < nvert; j = i++) {
		if ( ((verty[i]>testy) != (verty[j]>testy)) &&
		 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
		   c = !c;
	  }
	  return c;
	}
}

bool PnPoly::InsideSANTANA(double g_lat, double g_lon) const
{
	return pnpoly(8, p_sant_lon, p_sant_lat, g_lon, g_lat);
}

bool PnPoly::InsideMAPGEO15(double g_lat, double g_lon) const
{
	return pnpoly(7, p_mgeo_lon, p_mgeo_lat, g_lon, g_lat);
}
