
/////////////////////////////////////////////////////////////////////
//
// Autor    Andre Caceres Carrilho
// Data     25 jul 2022
//
/////////////////////////////////////////////////////////////////////

#include "bicubic.h"

double CubicSpline::Eval(double x) const{
	double y = 0.0;
	double z = x * x;
	
	y += _parms.vec[3];
	y += _parms.vec[2] * x;
	y += _parms.vec[1] * z;
	y += _parms.vec[0] * z * x;
	
	return y;	
}

void CubicSpline::Fit(
	const Point& p1, 
	const Point& p2, 
	const Point& p3, 
	const Point& p4)
{
    Matrix A;
	
	A.mat[0][0] = p1.x * p1.x * p1.x;
	A.mat[0][1] = p1.x * p1.x;
	A.mat[0][2] = p1.x;
	A.mat[0][3] = 1;
	
	A.mat[1][0] = p2.x * p2.x * p2.x;
	A.mat[1][1] = p2.x * p2.x;
	A.mat[1][2] = p2.x;
	A.mat[1][3] = 1;
	
	A.mat[2][0] = p3.x * p3.x * p3.x;
	A.mat[2][1] = p3.x * p3.x;
	A.mat[2][2] = p3.x;
	A.mat[2][3] = 1;
	
	A.mat[3][0] = p4.x * p4.x * p4.x;
	A.mat[3][1] = p4.x * p4.x;
	A.mat[3][2] = p4.x;
	A.mat[3][3] = 1;
		
    Vector b;
	
	b.vec[0] = p1.y;
	b.vec[1] = p2.y;
	b.vec[2] = p3.y;
	b.vec[3] = p4.y;
   
    _parms = A.Solve(b);
}
