
/////////////////////////////////////////////////////////////////////
//
// DNIT - Departamento Nacional de Infraestrutura de Transportes
// STE - Servicos Tecnicos de Engenharia, SA
//
// Autor    Andre Caceres Carrilho
// Contato  andrecarrilho@ste-simemp.com.br
// Data     25 jul 2022
//
/////////////////////////////////////////////////////////////////////

#include "mat4.h"
#include <cmath>

///////////////////////////////////////////////////////////////////////////////////////
//
// Vector 
//
///////////////////////////////////////////////////////////////////////////////////////
Vector::Vector(void)
    : x(0.0)
    , y(0.0)
    , z(0.0)
    , w(0.0)
{ }
Vector::Vector(double _x, double _y, double _z, double _w)
    : x(_x)
    , y(_y)
    , z(_z)
    , w(_w)
{ }
Vector& Vector::operator = (const Vector& b){
    x=b.x; y=b.y; z=b.z; w=b.w;
    return *this;
}
Vector& Vector::operator += (const Vector& b){
    x+=b.x; y+=b.y; z+=b.z; w+=b.w;
    return *this;
}
Vector& Vector::operator -= (const Vector& b){
    x-=b.x; y-=b.y; z-=b.z; w-=b.w;
    return *this;
}
Vector Vector::operator + (const Vector& b) const{
    return Vector(x+b.x, y+b.y, z+b.z, w+b.w);
}
Vector Vector::operator - (const Vector& b) const{
    return Vector(x-b.x, y-b.y, z-b.z, w-b.w);
}
Vector Vector::operator * (float scalar) const{
	return Vector(x*scalar, y*scalar, z*scalar, w*scalar);
}
double Vector::LengthSqr(void) const{
	return x*x + y*y + z*z;
}
double Vector::Length(void) const{
	return std::sqrt(LengthSqr());
}
double Vector::Norm(void) const{
	return Length();
}
Vector Vector::Unit(void) const{
	double inorm = 1.0 / Length();
	return Vector(x*inorm, y*inorm, z*inorm);
}
double Vector::DistSqr(const Vector& b) const{
	double dx = x-b.x;
	double dy = y-b.y;
	double dz = z-b.z;
	return dx*dx + dy*dy + dz*dz;
}
double Vector::Dist(const Vector& b) const{
	return std::sqrt(DistSqr(b));
}
double Vector::Dot(const Vector& b) const{
    return (x*b.x) + (y*b.y) + (z*b.z);
}
Vector Vector::Cross(const Vector& b) const{
    Vector result;

    result.x = ( y * b.z ) - ( z * b.y );
    result.y = ( z * b.x ) - ( x * b.z );
    result.z = ( x * b.y ) - ( y * b.x );

    return result;
}
std::ostream& operator << (std::ostream& os, const Vector& b)
{
    return os << "\t" << b.x << "\t" << b.y << "\t" << b.z << "\t" << b.w;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Matrix 4x4
//
///////////////////////////////////////////////////////////////////////////////////////
Matrix::Matrix(void)
{
    SetZero();
}
Matrix& Matrix::operator = (const Matrix& b)
{
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        mat[i][j] = b.mat[i][j];
    } }

    return *this;
}

Matrix& Matrix::operator += (const Matrix& b) {
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        mat[i][j] += b.mat[i][j];
    } }
	
	return *this;
}
Matrix& Matrix::operator -= (const Matrix& b) {
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        mat[i][j] -= b.mat[i][j];
    } }
	
	return *this;
}
Matrix Matrix::operator + (const Matrix& b) const{
	Matrix result;
	
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        result.mat[i][j] = mat[i][j] + b.mat[i][j];
    } }
	
	return result;
}
Matrix Matrix::operator - (const Matrix& b) const{
	Matrix result;
	
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        result.mat[i][j] = mat[i][j] - b.mat[i][j];
    } }
	
	return result;
}	
Matrix Matrix::operator * (const Matrix& b) const{
	Matrix result;
	
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
	for (int k = 0; k < 4; ++k) {
        result.mat[i][j] += mat[i][k] * b.mat[k][j];
    } } }
	
	return result;
}	
Vector Matrix::operator * (const Vector& b) const
{
    Vector result;

    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        result.vec[i] += mat[i][j] * b.vec[j];
    } }

    return result;
}
Matrix Matrix::operator * (float scalar) const
{
    Matrix result;

    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        result.mat[i][j] = mat[i][j] * scalar;
    } }

    return result;
}
void Matrix::SetZero(void)
{
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        mat[i][j] = 0.0;
    } }
}
void Matrix::SetIdentity(void)
{
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        mat[i][j] = i == j ? 1.0 : 0.0;
    } }
}
Matrix Matrix::Trs(void) const
{
	Matrix result;
	
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        result.mat[i][j] = mat[j][i];
    } }
	
	return result;
}
Matrix Matrix::Inv(void) const
{
    Matrix result;

    double det2_01_01 = ( mat[0][0] * mat[1][1] ) - ( mat[0][1] * mat[1][0] );
    double det2_01_02 = ( mat[0][0] * mat[1][2] ) - ( mat[0][2] * mat[1][0] );
    double det2_01_03 = ( mat[0][0] * mat[1][3] ) - ( mat[0][3] * mat[1][0] );
    double det2_01_12 = ( mat[0][1] * mat[1][2] ) - ( mat[0][2] * mat[1][1] );
    double det2_01_13 = ( mat[0][1] * mat[1][3] ) - ( mat[0][3] * mat[1][1] );
    double det2_01_23 = ( mat[0][2] * mat[1][3] ) - ( mat[0][3] * mat[1][2] );

    double det2_03_01 = ( mat[0][0] * mat[3][1] ) - ( mat[0][1] * mat[3][0] );
    double det2_03_02 = ( mat[0][0] * mat[3][2] ) - ( mat[0][2] * mat[3][0] );
    double det2_03_03 = ( mat[0][0] * mat[3][3] ) - ( mat[0][3] * mat[3][0] );
    double det2_03_12 = ( mat[0][1] * mat[3][2] ) - ( mat[0][2] * mat[3][1] );
    double det2_03_13 = ( mat[0][1] * mat[3][3] ) - ( mat[0][3] * mat[3][1] );
    double det2_03_23 = ( mat[0][2] * mat[3][3] ) - ( mat[0][3] * mat[3][2] );

    double det2_13_01 = ( mat[1][0] * mat[3][1] ) - ( mat[1][1] * mat[3][0] );
    double det2_13_02 = ( mat[1][0] * mat[3][2] ) - ( mat[1][2] * mat[3][0] );
    double det2_13_03 = ( mat[1][0] * mat[3][3] ) - ( mat[1][3] * mat[3][0] );
    double det2_13_12 = ( mat[1][1] * mat[3][2] ) - ( mat[1][2] * mat[3][1] );
    double det2_13_13 = ( mat[1][1] * mat[3][3] ) - ( mat[1][3] * mat[3][1] );
    double det2_13_23 = ( mat[1][2] * mat[3][3] ) - ( mat[1][3] * mat[3][2] );

    result.mat[0][0] = -
		(
			( mat[2][1] * det2_13_23 ) -
			( mat[2][2] * det2_13_13 ) +
			( mat[2][3] * det2_13_12 )
		);

    result.mat[0][1] =
        (
            ( mat[2][1] * det2_03_23 ) -
            ( mat[2][2] * det2_03_13 ) +
            ( mat[2][3] * det2_03_12 )
        );

    result.mat[0][2] =
        (
            ( mat[3][1] * det2_01_23 ) -
            ( mat[3][2] * det2_01_13 ) +
            ( mat[3][3] * det2_01_12 )
        );

    result.mat[0][3] = -
		(
			( mat[2][1] * det2_01_23 ) -
			( mat[2][2] * det2_01_13 ) +
			( mat[2][3] * det2_01_12 )
		);

    result.mat[1][0] =
        (
            ( mat[2][0] * det2_13_23 ) -
            ( mat[2][2] * det2_13_03 ) +
            ( mat[2][3] * det2_13_02 )
        );

    result.mat[1][1] = -
		(
			( mat[2][0] * det2_03_23 ) -
			( mat[2][2] * det2_03_03 ) +
			( mat[2][3] * det2_03_02 )
		);

    result.mat[1][2] = -
		(
			( mat[3][0] * det2_01_23 ) -
			( mat[3][2] * det2_01_03 ) +
			( mat[3][3] * det2_01_02 )
		);

    result.mat[1][3] =
        (
            ( mat[2][0] * det2_01_23 ) -
            ( mat[2][2] * det2_01_03 ) +
            ( mat[2][3] * det2_01_02 )
        );

    result.mat[2][0] = -
		(
			( mat[2][0] * det2_13_13 ) -
			( mat[2][1] * det2_13_03 ) +
			( mat[2][3] * det2_13_01 )
		);

    result.mat[2][1] =
        (
            ( mat[2][0] * det2_03_13 ) -
            ( mat[2][1] * det2_03_03 ) +
            ( mat[2][3] * det2_03_01 )
        );

    result.mat[2][2] =
        (
            ( mat[3][0] * det2_01_13 ) -
            ( mat[3][1] * det2_01_03 ) +
            ( mat[3][3] * det2_01_01 )
        );

    result.mat[2][3] = -
		(
			( mat[2][0] * det2_01_13 ) -
			( mat[2][1] * det2_01_03 ) +
			( mat[2][3] * det2_01_01 )
		);

    result.mat[3][0] =
        (
            ( mat[2][0] * det2_13_12 ) -
            ( mat[2][1] * det2_13_02 ) +
            ( mat[2][2] * det2_13_01 )
        );

    result.mat[3][1] = -
		(
			( mat[2][0] * det2_03_12 ) -
			( mat[2][1] * det2_03_02 ) +
			( mat[2][2] * det2_03_01 )
		);

    result.mat[3][2] = -
		(
			( mat[3][0] * det2_01_12 ) -
			( mat[3][1] * det2_01_02 ) +
			( mat[3][2] * det2_01_01 )
		);

    result.mat[3][3] =
        (
            ( mat[2][0] * det2_01_12 ) -
            ( mat[2][1] * det2_01_02 ) +
            ( mat[2][2] * det2_01_01 )
        );

    double determinant =
        (
            ( result.mat[1][3] * mat[3][1] ) +
            ( result.mat[0][3] * mat[3][0] ) +
            ( result.mat[2][3] * mat[3][2] ) +
            ( result.mat[3][3] * mat[3][3] )
        );

    double inverseDeterminant = 1.0 / determinant;

    return result * inverseDeterminant;
}

double Matrix::Det(void) const
{
    double det2_01_01 = ( mat[0][0] * mat[1][1] ) - ( mat[0][1] * mat[1][0] );
    double det2_01_02 = ( mat[0][0] * mat[1][2] ) - ( mat[0][2] * mat[1][0] );
    double det2_01_03 = ( mat[0][0] * mat[1][3] ) - ( mat[0][3] * mat[1][0] );
    double det2_01_12 = ( mat[0][1] * mat[1][2] ) - ( mat[0][2] * mat[1][1] );
    double det2_01_13 = ( mat[0][1] * mat[1][3] ) - ( mat[0][3] * mat[1][1] );
    double det2_01_23 = ( mat[0][2] * mat[1][3] ) - ( mat[0][3] * mat[1][2] );

    double det3_201_012 =
        (
            ( mat[2][0] * det2_01_12 ) -
            ( mat[2][1] * det2_01_02 ) +
            ( mat[2][2] * det2_01_01 )
        );

    double det3_201_013 =
        (
            ( mat[2][0] * det2_01_13 ) -
            ( mat[2][1] * det2_01_03 ) +
            ( mat[2][3] * det2_01_01 )
        );

    double det3_201_023 =
        (
            ( mat[2][0] * det2_01_23 ) -
            ( mat[2][2] * det2_01_03 ) +
            ( mat[2][3] * det2_01_02 )
        );

    double det3_201_123 =
        (
            ( mat[2][1] * det2_01_23 ) -
            ( mat[2][2] * det2_01_13 ) +
            ( mat[2][3] * det2_01_12 )
        );

    return
        (
            ( det3_201_023 * mat[3][1] ) -
            ( det3_201_123 * mat[3][0] ) -
            ( det3_201_013 * mat[3][2] ) +
            ( det3_201_012 * mat[3][3] )
        );
}
Vector Matrix::Solve(const Vector& b) const
{
    return Inv() * b;
}

std::ostream& operator << (std::ostream& os, const Matrix& b)
{
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        os << "\t" << b.mat[i][j]; }
        os << std::endl;
    }
    return os;
}

