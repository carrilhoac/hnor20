
#include "hnor20.h"

#include <cmath>
#include <fstream>
#include <iostream>

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
{
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

    delta = GetFactorNearest(-20.414736, -49.975325 + 360.0) - ( -7.64);
    delta_s += std::abs(delta);
    std::cout << delta << std::endl;
    delta = GetFactorNearest(-29.907579, -51.823868 + 360.0) - (  5.95);
    delta_s += std::abs(delta);
    std::cout << delta << std::endl;
    delta = GetFactorNearest(-20.194407, -43.618629 + 360.0) - ( -5.64);
    delta_s += std::abs(delta);
    std::cout << delta << std::endl;
    delta = GetFactorNearest(-20.895839, -54.868729 + 360.0) - (  2.83);
    delta_s += std::abs(delta);
    std::cout << delta << std::endl;
    delta = GetFactorNearest(-25.530668, -51.774829 + 360.0) - (  4.06);
    delta_s += std::abs(delta);
    std::cout << delta << std::endl;
    delta = GetFactorNearest(-16.090905, -57.711762 + 360.0) - (  8.35);
    delta_s += std::abs(delta);
    std::cout << delta << std::endl;
    delta = GetFactorNearest(-11.735657, -49.138925 + 360.0) - (-16.30);
    delta_s += std::abs(delta);
    std::cout << delta << std::endl;
    delta = GetFactorNearest(-8.593642 , -61.863849 + 360.0) - (  6.45);
    delta_s += std::abs(delta);
    std::cout << delta << std::endl;
    delta = GetFactorNearest(-9.905970 , -67.738284 + 360.0) - ( 24.05);
    delta_s += std::abs(delta);
    std::cout << delta << std::endl;

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

	for (int i = 0; i < _nrows; ++i)
	for (int j = 0; j < _ncols; ++j)
	{{
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
