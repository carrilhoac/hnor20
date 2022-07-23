

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

