
/////////////////////////////////////////////////////////////////////
//
// DNIT - Departamento Nacional de Infraestrutura de Transportes
// STE - Servicos Tecnicos de Engenharia, SA
//
// Autor    Andre Caceres Carrilho
// Contato  andrecarrilho@ste-simemp.com.br
// Data     23 jul 2022
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

PointEntry::PointEntry()
    : factor(0.0)

    , i_row(-1)
    , i_col(-1)

    , d_row(-1.0)
    , d_col(-1.0)

    , dif_row(0.0)
    , dif_col(0.0)

    , d_lat(0.0)
    , d_lon(0.0)
{ }

double C_hnor::GetFactor(double g_lat, double g_lon, INTERP_METHOD m) const
{
    g_lon += 360.0;
    double r = 0.0;

    switch (m)
    {
        case INTERP_NEAREST:
            r = GetFactorNearest(g_lat, g_lon);
        break;
        case INTERP_BILINEAR:
            r = GetFactorBilinear(g_lat, g_lon);
        break;
        case INTERP_BICUBIC:
            r = GetFactorBicubic(g_lat, g_lon);
        break;
        default:
            r = GetFactorBicubic(g_lat, g_lon);
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


double C_hnor::GetFactorNearest(double g_lat, double g_lon) const
{
    PointEntry p = GetEntry(g_lat, g_lon);

    p.i_row = int(p.d_row +0.5);
    p.i_col = int(p.d_col +0.5);

    p.factor = _fator[p.i_row][p.i_col];
    return p.factor;
}

double C_hnor::GetFactorBilinear(double g_lat, double g_lon) const
{
    PointEntry p = GetEntry(g_lat, g_lon);

    double Q00 = _fator[p.i_row   ][p.i_col   ];
    double Q01 = _fator[p.i_row   ][p.i_col +1];
    double Q10 = _fator[p.i_row +1][p.i_col   ];
    double Q11 = _fator[p.i_row +1][p.i_col +1];

    p.factor = _Interp_Bilinear(Q00, Q01, Q10, Q11, p.dif_col, p.dif_row);
    return p.factor;
}

double C_hnor::GetFactorBicubic(double g_lat, double g_lon) const
{
    PointEntry p = GetEntry(g_lat, g_lon);

	double QF[4][4];
	
	for (int i = 0, u = p.i_row -1; i < 4; ++i, ++u){
	for (int j = 0, v = p.i_col -1; j < 4; ++j, ++v){
		QF[i][j] = _fator[u][v];
	} }

    p.factor = _Interp_Bicubic(QF, p.dif_col, p.dif_row);
    return p.factor;
}

PointEntry C_hnor::GetEntry(int n_row, int n_col) const
{
    PointEntry p;

    if (!_InRange(n_row, n_col))
        return p;

    p.i_row = n_row;
    p.i_col = n_col;

    p.d_lat = _ilat - (p.i_row * _step);
    p.d_lon = _ilon - (p.i_col * _step);

    p.factor = _fator[p.i_row][p.i_col];
  //  std::cout << p.d_lat << " " << p.d_lon << std::endl;

    return p;
}
PointEntry C_hnor::GetEntry(double g_lat, double g_lon) const
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
  //  std::cout << p.i_row << " " << p.i_col << std::endl;

    return p;
}

void C_hnor::TestInterp()
{
    double delta_s = 0.0;
    double delta = 0.0;

    INTERP_METHOD m = INTERP_BILINEAR;
	
    delta = GetFactor(-20.414736, -49.975325, m) - ( -7.64);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-29.907579, -51.823868, m) - (  5.95);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-20.194407, -43.618629, m) - ( -5.64);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-20.895839, -54.868729, m) - (  2.83);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-25.530668, -51.774829, m) - (  4.06);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-16.090905, -57.711762, m) - (  8.35);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-11.735657, -49.138925, m) - (-16.30);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-8.593642 , -61.863849, m) - (  6.45);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-9.905970 , -67.738284, m) - ( 24.05);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
	
	
    delta = GetFactor(-32.540392 , -53.156549, m) - ( 11.17);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-28.653433 , -55.674432, m) - ( 10.35);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-27.344112 , -49.007961, m) - ( 1.58);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-26.688981 , -53.4969219, m) - ( 5.04);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-18.022189 , -52.922957, m) - ( -3.31);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-18.356500 , -46.159374, m) - ( -10.39);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-18.138242 , -40.956673, m) - ( -9.56);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-20.210832 , -40.837231, m) - ( -5.96);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;	
    delta = GetFactor(-24.027784 , -47.513957, m) - ( -3.46);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-25.499264 , -49.394001, m) - ( 4.12);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-13.017753 , -53.024325, m) - ( -8.75);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = GetFactor(-21.323203 , -47.127257, m) - ( -5.49);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;

    std::cout << std::endl << (delta_s / 9.0) << std::endl;
}
void C_hnor::TestInRange()
{
#if 0
    GetEntry(  35, 132);
    GetEntry( 240, 55);
    GetEntry( 408, 315);
#endif
#if 0
    GetEntry(-21.38934, -48.704394 +360.0);
    GetEntry( 3.23532, -61.140571 +360.0);
    GetEntry(-39.93551, -68.1440844 +360.0);
    GetEntry(32.410675, -83.414223 +360.0);
#endif
#if 0
    if (_InRange(-21.38934, -48.704394 +360.0))
        std::cout << "ok1" << std::endl;
    if (_InRange( 3.23532, -61.140571 +360.0))
        std::cout << "ok2" << std::endl;
    if (!_InRange(-39.93551, -68.1440844 +360.0))
        std::cout << "ok3" << std::endl;
    if (!_InRange(32.410675, -83.414223 +360.0))
        std::cout << "ok4" << std::endl;
#endif
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

	//	std::cout << _fator[i][j] << std::endl;
	}}

	in_file.close();
	return true;
}

C_hnor::C_hnor(void)
	: _fator(nullptr)
{
	_MemAlloc();

	// TODO: calcular CRC32 do arquivo antes de ler

	_ReadTextFile("hgeoHNOR2020__IMBITUBA__fator-conversao.txt");


	std::cout << std::fixed;
#if 0
	std::cout << _step << std::endl;
	std::cout << _ilon << std::endl;
	std::cout << _ilat << std::endl;
#endif
#if 0
    std::cout << _bblon[0] << " " << _bblon[1] << std::endl;
    std::cout << _bblat[0] << " " << _bblat[1] << std::endl;
#endif
}

C_hnor::~C_hnor(void)
{
	_MemFree();
}

void C_hnor::_MemAlloc(void)
{
	_MemFree();

	_fator = new double * [ _nrows ];
	for (int i = 0; i < _nrows; ++i)
	{
		_fator[i] = new double[ _ncols ];
	}
}

void C_hnor::_MemFree(void)
{
	if (_fator)
	{
		for (int i = 0; i < _nrows; ++i)
		{
			if (_fator[i])
			{
				delete [] _fator[i];
				_fator[i] = nullptr;
			}
		}
		delete [] _fator;
		_fator = nullptr;
	}
}
