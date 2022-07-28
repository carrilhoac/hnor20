
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
		double 	 uncertainty;

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
	double **					_incer;

	// hgeoHNOR2020 grid constants
	static constexpr double     _istep      = 12.0;
	static constexpr double 	_step       = 1.0 / 12.0;

	int 						_ncols;
	int 						_nrows;

	// initial and final grid coordinates
	double 						_ilon;       
	double 						_ilat;        
					
	double 						_flon;      
	double 						_flat;      

	// axis aligned bounding box
	double     					_bblon[2];
	double     					_bblat[2];

private:
	void 						_SetGridImbituba(void);
	void 						_SetGridSantana(void);
	void 						_SetGridMapgeo(void);
	void 						_UpdateBoundingBox(void);
	
private:
	void 					    _MemAlloc(void);
	void 					    _MemFree(void);

	bool					    _ReadTextFile(const char *txt_file);
	bool					    _ReadBinFile(const char *txt_file);

	bool                        _InRange(double g_lat, double g_lon) const;
	bool                        _InRange(int n_row, int n_col) const;

private:
	double                      GetNearest(double g_lat, double g_lon, bool _is_uncert = false) const;
	double                      GetBilinear(double g_lat, double g_lon, bool _is_uncert = false) const;
	double                      GetBicubic(double g_lat, double g_lon, bool _is_uncert = false) const;

	PointEntry                  GetEntry(double g_lat, double g_lon) const;
	PointEntry                  GetEntry(int n_row, int n_col) const;

public:
	C_hnor(void);
	~C_hnor(void);

	double                      GetFactor(double g_lat, double g_lon, INTERP_METHOD m = INTERP_BICUBIC) const;
	double                      GetUncertainty(double g_lat, double g_lon, INTERP_METHOD m = INTERP_BICUBIC) const;
};

#endif
