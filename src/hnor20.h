
/////////////////////////////////////////////////////////////////////
//
// Autor    Andre Caceres Carrilho
// Data     22 jul 2022
//
/////////////////////////////////////////////////////////////////////

#ifndef __IBGE_HGEOHNOR2020_H__
#define __IBGE_HGEOHNOR2020_H__

#include <string>
#include <iostream>

enum INTERP_METHOD
{
    INTERP_NEAREST,
    INTERP_BILINEAR,
    INTERP_BICUBIC
};

enum GRID_NAME
{
	GRID_IMBITUBA,
	GRID_SANTANA,
	GRID_MAPGEO2015
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Coordinates: LAT LON in decimal degrees
// LAT:  -90.0  to   +90.0   equator
// LON: -180.0  to  +180.0   greenwich
//
////////////////////////////////////////////////////////////////////////////////////////////////
class CGrid {
private:
	struct PointEntry
	{
		double   	factor;
		double 	 	uncertainty;
	
		int      	i_row;
		int      	i_col;
	
		double   	d_row;
		double   	d_col;
	
		double   	dif_row;
		double   	dif_col;
	
		double   	d_lat;
		double   	d_lon;

		PointEntry(void);
	};

private:
    // geoid model conversion factors
	double **				    _fator;
	double **					_incer;

	GRID_NAME					_name;
	
	int							_offset;

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

	// hgeoHNOR2020 grid constants
	static constexpr double     _istep      = 12.0;
	static constexpr double 	_step       = 1.0 / 12.0;
	
private:
	int 						_OffsetImbituba()const;
	int							_OffsetSantana()const;
	int							_OffsetMapgeo()const;
	
	void 						_SetGridImbituba(void);
	void 						_SetGridSantana(void);
	void 						_SetGridMapgeo(void);
	void 						_UpdateBoundingBox(void);
	
private:
	void 					    _MemAlloc(void);
	void 					    _MemFree(void);

	bool					    _ReadBinFile(const char *txt_file);

	bool                        _InRange(double g_lat, double g_lon) const;
	bool                        _InRange(int n_row, int n_col) const;

private:
	double                      GetNearest(double g_lat, double g_lon, bool _is_uncert = false) const;
	double                      GetBilinear(double g_lat, double g_lon, bool _is_uncert = false) const;
	double                      GetBicubic(double g_lat, double g_lon, bool _is_uncert = false) const;

	PointEntry                  GetEntry(double g_lat, double g_lon) const;

public:
	CGrid(GRID_NAME g_name);
	~CGrid(void);

	double                      GetValue(double g_lat, double g_lon, bool get_uncert, INTERP_METHOD m = INTERP_BICUBIC) const;
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class PointResult  {
public:
	double			_lat;			 // WGS84
	double			_lon;
		
	double 			_utm_E;
	double			_utm_N;
	char			_utm_F[4];
		
	double 			_fator;
	double			_incer;
	std::string 	_model;

public:
	static std::string	GetHeaderTXT();
	static std::string	GetHeaderCSV();
	
	std::string		ToStringTXT()const;
	std::string		ToStringCSV()const;
	
	friend std::ostream& operator << (std::ostream& os, const PointResult& pt);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class HNOR {
private:
	CGrid	_imbituba;
	CGrid  	_santana;
	CGrid 	_mapgeo15;
	
public:
	HNOR(void);

	PointResult		Get(double g_lat, double g_lon, INTERP_METHOD m = INTERP_BICUBIC) const;
};

#endif
