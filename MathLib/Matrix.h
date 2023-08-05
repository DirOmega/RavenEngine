#ifndef ENGINE_MATH_MATRIX_H
#define ENGINE_MATH_MATRIX_H

// SIMD includes
#include "Enum.h"
#include "Vect.h"

#include <xmmintrin.h>
#include <smmintrin.h> 

//TODO:: add align calss so that if someone fucks up it's still going to get aligned with a standard new call
class Matrix
{
public:

	//constructors
	Matrix();

	~Matrix();

	Matrix(const __m128 & vt0,
		const __m128 & vt1,
		const __m128 & vt2,
		const __m128 & vt3);

	Matrix(MatrixTransType, float x, float y, float z);

	Matrix(MatrixTransType, Vect v);

	Matrix(MatrixScaleType, Vect v);

	Matrix(MatrixScaleType, float x, float y, float z);

	Matrix(const Matrix &t);

	Matrix(const Vect &tV0,
		const Vect &tV1,
		const Vect &tV2,
		const Vect &tV3);

	Matrix(MatrixSpecialType t);

	Matrix(RotType type, float radians);

	//Accessors (getters amd setters)

	float& operator[] (m0_enum);
	float& operator[] (m1_enum);
	float& operator[] (m2_enum);
	float& operator[] (m3_enum);
	float& operator[] (m4_enum);
	float& operator[] (m5_enum);
	float& operator[] (m6_enum);
	float& operator[] (m7_enum);
	float& operator[] (m8_enum);
	float& operator[] (m9_enum);
	float& operator[] (m10_enum);
	float& operator[] (m11_enum);
	float& operator[] (m12_enum);
	float& operator[] (m13_enum);
	float& operator[] (m14_enum);
	float& operator[] (m15_enum);

	float  operator[] (m0_enum)  const;
	float  operator[] (m1_enum)  const;
	float  operator[] (m2_enum)  const;
	float  operator[] (m3_enum)  const;
	float  operator[] (m4_enum)  const;
	float  operator[] (m5_enum)  const;
	float  operator[] (m6_enum)  const;
	float  operator[] (m7_enum)  const;
	float  operator[] (m8_enum)  const;
	float  operator[] (m9_enum)  const;
	float  operator[] (m10_enum) const;
	float  operator[] (m11_enum) const;
	float  operator[] (m12_enum) const;
	float  operator[] (m13_enum) const;
	float  operator[] (m14_enum) const;
	float  operator[] (m15_enum) const;

	float& m0();
	float& m1();
	float& m2();
	float& m3();
	float& m4();
	float& m5();
	float& m6();
	float& m7();
	float& m8();
	float& m9();
	float& m10();
	float& m11();
	float& m12();
	float& m13();
	float& m14();
	float& m15();

	float m0()  const;
	float m1()  const;
	float m2()  const;
	float m3()  const;
	float m4()  const;
	float m5()  const;
	float m6()  const;
	float m7()  const;
	float m8()  const;
	float m9()  const;
	float m10() const;
	float m11() const;
	float m12() const;
	float m13() const;
	float m14() const;
	float m15() const;

	Vect v0() const;
	Vect v1() const;
	Vect v2() const;
	Vect v3() const;

	Vect get(MatrixRowType row);

	//const and ref everything below
	void set(Vect v0, Vect v1, Vect v2, Vect v3);

	//TODO: check the performance impact of set_ps1 vs set_ps
	void set(MatrixSpecialType t);

	void set(MatrixRowType row, Vect inV);

	void set(MatrixTransType, float x, float y, float z);

	//TODO: check the performance differnce between set and setr
	void set(MatrixScaleType, float x, float y, float z);

	void set(MatrixScaleType, Vect v);

	void set(MatrixTransType, Vect v);

	void set(RotType type, float f);

	//Operators

	Matrix& operator = (const Matrix& m);

	Matrix operator + ();

	Matrix operator - ();

	Matrix& operator += (const Matrix& m);

	Matrix& operator -= (const Matrix& m);

	Matrix operator - (const Matrix& m);

	Matrix operator + (const Matrix& m);

	Vect Matrix::operator * (const Vect &v);

	Matrix& operator *= (const float& scaleFactor);

	Matrix operator * (const float& scaleFactor) const;

	Matrix operator * (const Matrix& m2);

	Matrix operator *= (const Matrix& m2);

	friend Matrix operator * (const float& scaleFactor, const Matrix& m)
	{
		return m*scaleFactor;
	}

	//Methods

	void inv();

	Matrix getInv();

	Matrix getT() const;

	float det();

	void T();

	bool isIdentity(float tolerance);

	bool isIdentity();

	bool isEqual(const Matrix& rhs);


	//extensions not yet with unit tests


private:

	Matrix adj();

	float det3x3(
		float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);

	float det2x2(float m0, float m1, float m2, float m3);

	bool identityV0(const float& tolerance);

	bool identityV1(const float& tolerance);

	bool identityV2(const float& tolerance);

	bool identityV3(const float& tolerance);

	union
	{
		struct
		{
			Vect _v0;
			Vect _v1;
			Vect _v2;
			Vect _v3;
		};

		struct
		{
			float _m0;
			float _m1;
			float _m2;
			float _m3;
			float _m4;
			float _m5;
			float _m6;
			float _m7;
			float _m8;
			float _m9;
			float _m10;
			float _m11;
			float _m12;
			float _m13;
			float _m14;
			float _m15;
		};
	};
};


#endif

// ---  End of File ---------------
