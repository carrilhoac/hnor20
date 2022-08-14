
/////////////////////////////////////////////////////////////////////
//
// Autor    Andre Caceres Carrilho
// Data     25 jul 2022
//
/////////////////////////////////////////////////////////////////////

#ifndef __MATRIX44_H__
#define __MATRIX44_H__

#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////
//
// Vector 
//
///////////////////////////////////////////////////////////////////////////////////////
struct Vector{
public:
    union {
        double vec[4];
        struct {
            double x;
            double y;
            double z;
            double w;
        };
    };

    Vector(void);
    Vector(double _x, double _y, double _z, double _w = 1.0);

    Vector&     operator = (const Vector& b);

    Vector&     operator += (const Vector& b);
    Vector&     operator -= (const Vector& b);

    Vector      operator + (const Vector& b) const;
    Vector      operator - (const Vector& b) const;
	
	Vector		operator * (float scalar) const;

    // -------------- only XYZ -------------
    double      Dot(const Vector& b) const;
    Vector      Cross(const Vector& b) const;

    double      LengthSqr(void) const;
    double      Length(void) const;

    double      Norm(void) const;
    Vector      Unit(void) const;

    double      DistSqr(const Vector& b) const;
    double      Dist(const Vector& b) const;
    // ------------------------------------

    friend std::ostream& operator << (std::ostream& os, const Vector& b);
};


///////////////////////////////////////////////////////////////////////////////////////
//
// Matrix 4x4
//
///////////////////////////////////////////////////////////////////////////////////////
struct Matrix{
public:
    double mat[4][4];

public:
    Matrix(void);

    Matrix& 		operator = (const Matrix& b);
	
	Matrix&			operator += (const Matrix& b);
	Matrix&			operator -= (const Matrix& b);
	
	Matrix			operator + (const Matrix& b) const;
	Matrix			operator - (const Matrix& b) const;
	Matrix			operator * (const Matrix& b) const;
		
    Matrix 			operator * (float scalar) const;
    Vector 			operator * (const Vector& b) const;
			
	void			SetIdentity(void);
	void			SetZero(void);
	
    double  		Det(void) const;
    Matrix  		Inv(void) const;
	Matrix			Trs(void) const;
	
    Vector 			Solve(const Vector& b) const;

    friend std::ostream& operator << (std::ostream& os, const Matrix& b);
};

#endif 
