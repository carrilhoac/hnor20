
#ifndef __IBGE_HGEOHNOR2020_H__
#define __IBGE_HGEOHNOR2020_H__

#include <fstream>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Coordinates: LAT LON in decimal degrees
// LAT: -90.00  to  +90.00   equator
// LON:   0.00  to  360.00   greenwich
//
////////////////////////////////////////////////////////////////////////////////////////////////
class C_hnor {
private:
    struct PointEntry
    {
        int             i_row;
        int             i_col;

        double          d_lat;
        double          d_lon;

        double          factor;
    };
    struct InterpBilinear
    {
        PointEntry      p00, p01;
        PointEntry      p10, p11;
    };
    struct InterpBicubic
    {
        PointEntry      p00, p01, p02, p03;
        PointEntry      p10, p11, p12, p13;
        PointEntry      p20, p21, p22, p23;
        PointEntry      p30, p31, p32, p33;
    };
private:
    // geoid model conversion factors
	double **				    _fator;

	// hgeoHNOR2020 grid constants
	static constexpr int 		_ncols      = 540;
	static constexpr int 		_nrows      = 492;
	static constexpr double 	_step       = 1.0 / 12.0;

	static constexpr double     _eps        = 1e-8;

	// initial and final grid coordinates
	static constexpr double 	_ilon       = 285.0 + (1.0 / 24.0);         // 285.0416667
	static constexpr double 	_ilat       = 5.95  + (1.0 / 120.0);        //   5.9583333

	static constexpr double 	_flon       = 329.95 + (1.0 / 120.0);       // 329.9583333
	static constexpr double 	_flat       = -34.95 - (1.0 / 120.0);       // -34.9583333

	// axis aligned bounding box
	static constexpr double     _bblon[2]    = { _ilon, _flon };
	static constexpr double     _bblat[2]    = { _flat, _ilat };

private:
	void 					    _MemAlloc(void);
	void 					    _MemFree(void);

	bool					    _ReadTextFile(const char *txt_file);
	static bool				    _ReadDouble(std::ifstream& in_file, double *val = nullptr);

	bool                        _InRange(double g_lat, double g_lon) const;

public:
	C_hnor(void);
	~C_hnor(void);

	void                        TestInRange();

	double                      GetFactorNearest(double g_lat, double g_lon) const;
	double                      GetFactorBilinear(double g_lat, double g_lon) const;
	double                      GetFactorBicubic(double g_lat, double g_lon) const; // DEFAULT
	double                      GetFactorInvSqrDst(double g_lat, double g_lon) const;
};

#endif
