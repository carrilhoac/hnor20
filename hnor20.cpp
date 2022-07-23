
#include <fstream>
#include <iostream>

class C_hnor {
private:
	double **				    _fator;

	static constexpr int 		_ncols      = 540;
	static constexpr int 		_nrows      = 492;

	static constexpr double 	_step       = 1.0 / 12.0;

	static constexpr double 	_ilon       = 285.0 + (1.0 / 24.0);  // 285.0416667
	static constexpr double 	_ilat       = 5.95  + (1.0 / 120.0); //   5.9583333

private:
	void 					    _MemAlloc(void);
	void 					    _MemFree(void);

	bool					    _ReadTextFile(const char *txt_file);
	static bool				    _ReadDouble(std::ifstream& in_file, double *val = nullptr);

public:
	C_hnor(void);
	~C_hnor(void);
};

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
	_ReadTextFile("hgeoHNOR2020__IMBITUBA__fator-conversao.txt");
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

int main(int argc, char **argv)
{
    C_hnor grid;

	return 0;
}
