
/////////////////////////////////////////////////////////////////////
//
// DNIT - Departamento Nacional de Infraestrutura de Transportes
// STE - Servicos Tecnicos de Engenharia, SA
//
// Autor    Andre Caceres Carrilho
// Contato  andrecarrilho@ste-simemp.com.br
// Data     22 jul 2022
//
/////////////////////////////////////////////////////////////////////

#ifndef __IBGE_HGEOHNOR2020_H__
#define __IBGE_HGEOHNOR2020_H__

enum INTERP_METHOD
{
    INTERP_NEAREST,
    INTERP_BILINEAR,
    INTERP_BICUBIC
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Coordinates: LAT LON in decimal degrees
// LAT:  -90.0  to   +90.0   equator
// LON: -180.0  to  +180.0   greenwich
//
////////////////////////////////////////////////////////////////////////////////////////////////
class C_hnor {
private:
	struct PointEntry
	{
		double   factor;

		int      i_row;
		int      i_col;

		double   d_row;
		double   d_col;

		double   dif_row;
		double   dif_col;

		double   d_lat;
		double   d_lon;

		PointEntry(void);
	};

private:
    // geoid model conversion factors
	double **				    _fator;

	// hgeoHNOR2020 grid constants
	static constexpr int 		_ncols      = 540;
	static constexpr int 		_nrows      = 492;
	static constexpr double     _istep      = 12.0;
	static constexpr double 	_step       = 1.0 / 12.0;

	//static constexpr double     _eps        = 1e-8;

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
	bool					    _ReadBinFile(const char *txt_file);

	bool                        _InRange(double g_lat, double g_lon) const;
	bool                        _InRange(int n_row, int n_col) const;

private:
	double                      GetFactorNearest(double g_lat, double g_lon) const;
	double                      GetFactorBilinear(double g_lat, double g_lon) const;
	double                      GetFactorBicubic(double g_lat, double g_lon) const;

	PointEntry                  GetEntry(double g_lat, double g_lon) const;
	PointEntry                  GetEntry(int n_row, int n_col) const;

public:
	C_hnor(void);
	~C_hnor(void);

	double                      GetFactor(double g_lat, double g_lon, INTERP_METHOD m = INTERP_BICUBIC) const;
};

#endif
