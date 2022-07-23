
#include "hnor20.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  assuming
//
//  step = 1/12
//  1/step = 12
//
//  given   (i,j)  find   (lat,lon)
//
//  lon = ilon + j * step
//  lat = ilat - i * step
//
//  inverse
//
//  j * step = lon - ilon =>  j =   (lon - ilon) * (1 / step)
// -i * step = lat - ilat =>  i = - (lat - ilat) * (1 / step)
//
//  j = (lon - ilon) * 12
//  i = (ilat - lat) * 12
//
////////////////////////////////////////////////////////////////////////////////////////////////

PointEntry C_hnor::GetEntry(double g_lat, double g_lon) const
{
    PointEntry p;

    p.d_lat = g_lat;
    p.d_lon = g_lon;

    p.i_row = (_ilat - p.d_lat) * 12.0;
    p.i_col = (p.d_lon - _ilon) * 12.0;

    std::cout << p.i_row << " " << p.i_col << std::endl;

    return p;
}

void C_hnor::TestInRange()
{
    GetEntry(-21.38934, -48.704394 +360.0);
    GetEntry( 3.23532, -61.140571 +360.0);
    GetEntry(-39.93551, -68.1440844 +360.0);
    GetEntry(32.410675, -83.414223 +360.0);
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
bool C_hnor::_ReadDouble(std::ifstream& in_file, double *val)
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

	//_ReadTextFile("hgeoHNOR2020__IMBITUBA__fator-conversao.txt");


	std::cout << std::fixed;
#if 0
	std::cout << _step << std::endl;
	std::cout << _ilon << std::endl;
	std::cout << _ilat << std::endl;
#endif
#if 1
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
