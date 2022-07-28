
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

#include "hnor20.h"
#include "bicubic.h"

#include <cmath>
#include <fstream>
#include <iostream>

static double
_Interp_Bilinear(
        double Q00, double Q01,
        double Q10, double Q11,
        double dx, double dy)
{
    double QU = (dx * (Q01-Q00)) + Q00;
    double QV = (dx * (Q11-Q10)) + Q10;
    return (dy * (QV-QU)) + QU;
}

static double
_Interp_Bicubic(
        double Q[4][4],
        double dx, double dy)
{	
	Point p1 = { -1.0, 0.0 };
	Point p2 = {  0.0, 0.0 };
	Point p3 = {  1.0, 0.0 };
	Point p4 = {  2.0, 0.0 };
	
	CubicSpline spline;
	
	p1.y = Q[0][0];
	p2.y = Q[0][1];
	p3.y = Q[0][2];
	p4.y = Q[0][3];
	
	spline.Fit(p1, p2, p3, p4);	
	double QA = spline.Eval(dx);
	
	p1.y = Q[1][0];
	p2.y = Q[1][1];
	p3.y = Q[1][2];
	p4.y = Q[1][3];
	
	spline.Fit(p1, p2, p3, p4);	
	double QB = spline.Eval(dx);
	
	p1.y = Q[2][0];
	p2.y = Q[2][1];
	p3.y = Q[2][2];
	p4.y = Q[2][3];
	
	spline.Fit(p1, p2, p3, p4);	
	double QC = spline.Eval(dx);
	
	p1.y = Q[3][0];
	p2.y = Q[3][1];
	p3.y = Q[3][2];
	p4.y = Q[3][3];
	
	spline.Fit(p1, p2, p3, p4);	
	double QD = spline.Eval(dx);
	
	p1.y = QA;
	p2.y = QB;
	p3.y = QC;
	p4.y = QD;
	
	spline.Fit(p1, p2, p3, p4);		
    return spline.Eval(dy);
}

C_hnor::PointEntry::PointEntry()
    : factor(0.0)
	, uncertainty(0.0)

    , i_row(-1)
    , i_col(-1)

    , d_row(-1.0)
    , d_col(-1.0)

    , dif_row(0.0)
    , dif_col(0.0)

    , d_lat(0.0)
    , d_lon(0.0)
{ }

void C_hnor::_SetGridImbituba(void)
{
	_ncols = 540;
	_nrows = 492;
	
	_ilon  = 285.0 + (1.0 / 24.0);         // 285.0416667
	_ilat  = 5.95  + (1.0 / 120.0);        //   5.9583333

	_flon  = 329.95 + (1.0 / 120.0);       // 329.9583333
	_flat  = -34.95 - (1.0 / 120.0);       // -34.9583333
	
	_UpdateBoundingBox();
}
void C_hnor::_SetGridSantana(void)
{
	_ncols = 96;
	_nrows = 96;
	
	_ilon  =   5.45 + (1.0 / 120.0);       //   5.45833333
	_ilat  = 303.0  + (1.0 / 24.0);        // 303.0416667

	_flon  =  -2.45 - (1.0 / 120.0);       //  -2.4583333 
	_flat  = 310.95 + (1.0 / 120.0);       // 310.9583333
	
	_UpdateBoundingBox();
}
void C_hnor::_SetGridMapgeo(void)
{
	_ncols = 138;
	_nrows = 192;
	
	_ilon  = 285.0 + (1.0 / 24.0);			//  285.0416667
	_ilat  =  3.95 + (1.0 / 120.0);			//    3.9583333
	
	_flon  = 296.45 + (1.0 / 120.0); 		//  296.4583333
	_flat  = -11.95 - (1.0 / 120.0); 		//  -11.9583333
	
	_UpdateBoundingBox();
}
void C_hnor::_UpdateBoundingBox(void)
{
	_bblon[0] = _ilon;
	_bblon[1] = _flon;
	
	_bblat[0] = _flat;
	_bblat[1] = _ilat;
}

double C_hnor::GetFactor(double g_lat, double g_lon, INTERP_METHOD m) const
{
    g_lon += 360.0;
    double r = 0.0;

    switch (m)
    {
        case INTERP_NEAREST:
            r = GetNearest(g_lat, g_lon);
        break;
        case INTERP_BILINEAR:
            r = GetBilinear(g_lat, g_lon);
        break;
        case INTERP_BICUBIC:
            r = GetBicubic(g_lat, g_lon);
        break;
        default:
            r = GetBicubic(g_lat, g_lon);
        break;
    }

    return r;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  assuming
//
//  step = 1/12
//  1/step = 12
//
//  given   (i,j)  find   (lat,lon)
//
//  lat = ilat - i * step
//  lon = ilon + j * step
//
//  inverse
//
// -i * step = lat - ilat =>  i = - (lat - ilat) * (1 / step)
//  j * step = lon - ilon =>  j =   (lon - ilon) * (1 / step)
//
//  i = (ilat - lat) * 12
//  j = (lon - ilon) * 12
//
////////////////////////////////////////////////////////////////////////////////////////////////


double C_hnor::GetNearest(double g_lat, double g_lon, bool _is_uncert) const
{
    PointEntry p = GetEntry(g_lat, g_lon);

    p.i_row = int(p.d_row +0.5);
    p.i_col = int(p.d_col +0.5);

    p.factor = _fator[p.i_row][p.i_col];
	p.uncertainty = _incer[p.i_row][p.i_col];
	
	if (_is_uncert)
		return p.uncertainty;
	
    return p.factor;
}

double C_hnor::GetBilinear(double g_lat, double g_lon, bool _is_uncert) const
{
    PointEntry p = GetEntry(g_lat, g_lon);

	if (_is_uncert)
	{
		double Q00 = _incer[p.i_row   ][p.i_col   ];
		double Q01 = _incer[p.i_row   ][p.i_col +1];
		double Q10 = _incer[p.i_row +1][p.i_col   ];
		double Q11 = _incer[p.i_row +1][p.i_col +1];

		p.uncertainty = _Interp_Bilinear(Q00, Q01, Q10, Q11, p.dif_col, p.dif_row);
		return p.uncertainty;
	}
	
    double Q00 = _fator[p.i_row   ][p.i_col   ];
    double Q01 = _fator[p.i_row   ][p.i_col +1];
    double Q10 = _fator[p.i_row +1][p.i_col   ];
    double Q11 = _fator[p.i_row +1][p.i_col +1];

    p.factor = _Interp_Bilinear(Q00, Q01, Q10, Q11, p.dif_col, p.dif_row);
    return p.factor;
}

double C_hnor::GetBicubic(double g_lat, double g_lon, bool _is_uncert) const
{
    PointEntry p = GetEntry(g_lat, g_lon);

	double QF[4][4];
	
	if (_is_uncert)
	{		
		for (int i = 0, u = p.i_row -1; i < 4; ++i, ++u){
		for (int j = 0, v = p.i_col -1; j < 4; ++j, ++v){
			QF[i][j] = _incer[u][v];
		} }
		p.uncertainty = _Interp_Bicubic(QF, p.dif_col, p.dif_row);
		return p.uncertainty;
	}
	for (int i = 0, u = p.i_row -1; i < 4; ++i, ++u){
	for (int j = 0, v = p.i_col -1; j < 4; ++j, ++v){
		QF[i][j] = _fator[u][v];
	} }

    p.factor = _Interp_Bicubic(QF, p.dif_col, p.dif_row);
    return p.factor;
}

C_hnor::PointEntry C_hnor::GetEntry(int n_row, int n_col) const
{
    PointEntry p;

    if (!_InRange(n_row, n_col))
        return p;

    p.i_row = n_row;
    p.i_col = n_col;

    p.d_lat = _ilat - (p.i_row * _step);
    p.d_lon = _ilon - (p.i_col * _step);

    p.factor = _fator[p.i_row][p.i_col];
	p.uncertainty = _incer[p.i_row][p.i_col];
  //  std::cout << p.d_lat << " " << p.d_lon << std::endl;

    return p;
}
C_hnor::PointEntry C_hnor::GetEntry(double g_lat, double g_lon) const
{
    PointEntry p;

    if (!_InRange(g_lat, g_lon))
        return p;

    p.d_lat = g_lat;
    p.d_lon = g_lon;

    p.d_row = (_ilat - p.d_lat) * _istep;
    p.d_col = (p.d_lon - _ilon) * _istep;

    p.i_row = int(p.d_row);
    p.i_col = int(p.d_col);

    p.dif_row = p.d_row - std::floor(p.d_row);
    p.dif_col = p.d_col - std::floor(p.d_col);

    p.factor = _fator[p.i_row][p.i_col];
	p.uncertainty = _incer[p.i_row][p.i_col];
  //  std::cout << p.i_row << " " << p.i_col << std::endl;

    return p;
}

bool C_hnor::_InRange(double g_lat, double g_lon) const
{
    return g_lat > _bblat[0] && g_lat < _bblat[1]
        && g_lon > _bblon[0] && g_lon < _bblon[1];
}
bool C_hnor::_InRange(int n_row, int n_col) const
{
    return n_row >= 0 && n_row < _nrows
        && n_col >= 0 && n_col < _ncols;
}
bool _ReadDouble(std::ifstream& in_file, double *val = nullptr)
{
	if (in_file.good())
	{
		if (val)
		{
			in_file >> *val;
		}
		else
		{
			double _read_temp;
			in_file >> _read_temp;
		}
		return true;
	}
	return false;
}
bool C_hnor::_ReadTextFile(const char *txt_file)
{
	std::ifstream  in_file;
	in_file.open(txt_file);

	if (!in_file.good())
	{
		return false;
	}

	for (int i = 0; i < _nrows; ++i){
	for (int j = 0; j < _ncols; ++j){
		if (!_ReadDouble(in_file)) { return false; }
		if (!_ReadDouble(in_file)) { return false; }
		if (!_ReadDouble(in_file, &_fator[i][j])) { return false; }
	}}

	in_file.close();
	return true;
}
bool C_hnor::_ReadBinFile(const char *txt_file)
{
	std::ifstream  in_file;
	in_file.open(txt_file, std::ios::binary);

	if (!in_file.good())
	{
		return false;
	}

	for (int i = 0; i < _nrows; ++i){
	for (int j = 0; j < _ncols; ++j){
		float _value_read;
		in_file.read( reinterpret_cast<char*>(&_value_read), sizeof(float));
		_fator[i][j] = double(_value_read);
	}}

	for (int i = 0; i < _nrows; ++i){
	for (int j = 0; j < _ncols; ++j){
		float _value_read;
		in_file.read( reinterpret_cast<char*>(&_value_read), sizeof(float));
		_incer[i][j] = double(_value_read);
	}}
	
	in_file.close();
	return true;
}

C_hnor::C_hnor(void)
	: _fator(nullptr)
	, _incer(nullptr)
{
	_SetGridImbituba();
	
	_MemAlloc();

	// TODO: calcular CRC32 do arquivo antes de ler

	_ReadBinFile("hgeoHNOR2020.geoid");
	//_ReadTextFile("hgeoHNOR2020__IMBITUBA__fator-conversao.txt");
}

C_hnor::~C_hnor(void)
{
	_MemFree();
}

static void _GridMemFree(double **ptr, int nrow, int ncol)
{
	if (ptr)
	{
		for (int i = 0; i < nrow; ++i)
		{
			if (ptr[i])
			{
				delete [] ptr[i];
				ptr[i] = nullptr;
			}
		}
		delete [] ptr;
		ptr = nullptr;
	}
}
static double **_GridMemAlloc(int nrow, int ncol)
{
	double **ptr = new double * [ nrow ];
	for (int i = 0; i < nrow; ++i)
	{
		ptr[i] = new double[ ncol ];
	}
	return ptr;
}

void C_hnor::_MemAlloc(void)
{
	_MemFree();	
	_fator = _GridMemAlloc(_nrows, _ncols);
	_incer = _GridMemAlloc(_nrows, _ncols);
}

void C_hnor::_MemFree(void)
{
	_GridMemFree(_fator, _nrows, _ncols);
	_GridMemFree(_incer, _nrows, _ncols);
}
