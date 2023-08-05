#pragma once

#include "Matrix.h"

//------------------------------------------------------------------------
// 
//  This class contains functions and data structure for a 4-dimensional
//  Quaternions.  The quaternions are internally stored in terms of the
//  imaginary components (x,y,z) and the real components (real).
// 
//  In the Math library, quaternions can be thought of as "rotational"
//  matrices, everything that you can represent with rotational matrices
//  you can use with quaternions.  Such as quaternion concatenation,
//  inverses, transpose, normalize, multiplying with matrices, and with
//  vectors.
// 
//------------------------------------------------------------------------

class Quat final : public Align16
{
public:

	Quat();
	Quat(const Quat& tmp);
	~Quat();

	Quat(const __m128& mq);
	Quat(const Vect& vcomponent, const float& realcomponent);
	Quat(const float& x,const float& y,const float& z,const float& w); 
	Quat(const Matrix& m);
	Quat(RotType, const float& f);
	Quat(RotAxisAngleType, const Vect& vect, float radian);
	Quat(RotOrientType, const Vect& vect, const Vect& vect2);
	Quat(Rot3AxisType, const float& xrot, const float& yrot, const float& zrot);

	void set(const float& x,const float& y,const float& z,const float& w);
	void set(const __m128& tmp);
	void set(const Matrix& in);
	void set(const Vect& vcomponent, const float& realcomponent);
	void set(RotAxisAngleType, const Vect& vect, float radian);
	void set(RotOrientType, const Vect& vect, const Vect& vect2); //may not be needed
	//void set(RotOrientType, const Vect& vect, float radian);
	void set(Rot3AxisType, const float& xrot, const float& yrot, const float& zrot);

	void set(MatrixSpecialType);
	void set(RotType type, float radians);

	void setVect(const Vect& vComponent);
	void getVect(Vect& vOut) const;
	void getAxis(Vect& vOut) const;
	
	float& operator[] (x_enum);
	float& operator[] (y_enum);
	float& operator[] (z_enum);
	float& operator[] (w_enum);

	float operator[] (x_enum) const;
	float operator[] (y_enum) const;
	float operator[] (z_enum) const;
	float operator[] (w_enum) const;

	float qx() const;
	float qy() const;
	float qz() const;
	float real() const;

	float& qx();
	float& qy();
	float& qz();
	float& real();

	Quat& operator = (const Quat&tmp);

	Quat operator - () const;

	Quat operator + () const;

	Quat operator + (const float& f) const;

	Quat& operator += (const float& f);

	Quat& operator += (const Quat &tmp);

	Quat& operator -= (const Quat &tmp);

	Quat& operator -= (const float& f);

	Quat operator - (const Quat& tmp) const;

	Quat operator - (const float& f) const;

	Quat operator + (const Quat& tmp) const;

	Quat operator / (const Quat &tmp) const;

	Quat& operator /= (const Quat &tmp);

	Quat operator / (const float& f) const;

	Quat& operator /= (const float& f);

	Quat operator * (const Quat& tmp) const;

	Quat operator * (const float& f) const;

	Quat operator *= (const Quat& tmp);

	Quat& operator *= (const float& s);

	Matrix operator * (const Matrix& tmp) const;

	Quat& operator *= (const Matrix&);

	//Quat operator * (const Matrix&) const;

	//operator __m128() const;

	friend Quat operator + (const float& f, const Quat& q)
	{
		return q + f;
	}

	friend Quat operator - (const float& f, const Quat& q)
	{
		return -q + f;
	}
	
	friend Quat operator * (const float& f, const Quat& q)
	{
		return q * f;
	}

	friend Quat operator / (const float& f, const Quat& q)
	{
		return Quat(_mm_div_ps(_mm_set_ps1(f), q._mq));
		//return //q / f;
	}

	/*friend Quat operator * (const Matrix& m, const Quat& q)
	{
		return q * m;
	}*/

	Quat multByElement(const Quat& tmp) const;
	
	bool isEqual   (const Quat& tmp, const float& error) const;
	bool isNegEqual(const Quat& rhs, const float& error) const;
	bool isEquivalent(const Quat& rhs, const float& error) const;

	bool isConjugateEqual(const Quat& rhs, const float& error) const;
	bool isIdentity(const float& epsilon) const;
	bool isNormalized(const float& epsilon) const;
	bool isZero(const float& epsilon) const;

	float mag() const;
	float magSquared() const;
	float invMag() const;
	float dot(const Quat& tmp) const;
	float getAngle() const;

	Quat getConj() const;
	Quat& conj();

	Quat getT() const;
	Quat& T();

	Quat& norm();
	Quat getNorm() const;

	Quat getInv() const;
	Quat& inv();

	void Lqcvq(const Vect& vin, Vect& vout) const;
	void Lqvqc(const Vect& vin, Vect& vout) const;

private:

	// anonymous union
	union
	{
		__m128	_mq;

		// anonymous struct
		struct
		{
			float _qx;
			float _qy;
			float _qz;
			float _qw;
		};
	};
};

// ---  End of File ---------------
