#pragma warning (push)
#pragma warning (disable :4201)

#include <math.h>

#pragma warning (pop)


#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Enum.h"
#include "MathEngine.h"
#include <assert.h>

// Do your magic here

//constructors
Matrix::Matrix() : _v0({ 0 }), _v1({ 0 }), _v2({ 0 }), _v3({ 0 }) {};

Matrix::~Matrix() {};

Matrix::Matrix(const __m128 & vt0, const __m128 & vt1, const __m128 & vt2, const __m128 & vt3)
	: _v0(vt0), _v1(vt1), _v2(vt2), _v3(vt3) {};

Matrix::Matrix(MatrixTransType, float x, float y, float z) :
	_v0(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f)),
	_v1(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f)),
	_v2(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f)),
	_v3(_mm_setr_ps(x, y, z, 1.0f)) {};

Matrix::Matrix(MatrixTransType, Vect v) :
	_v0(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f)),
	_v1(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f)),
	_v2(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f)),
	_v3(v) {};

Matrix::Matrix(MatrixScaleType, Vect v) :
	_v0(_mm_setr_ps(v.x(), 0.0f, 0.0f, 0.0f)),
	_v1(_mm_setr_ps(0.0f, v.y(), 0.0f, 0.0f)),
	_v2(_mm_setr_ps(0.0f, 0.0f, v.z(), 0.0f)),
	_v3(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f)) {};

Matrix::Matrix(MatrixScaleType, float x, float y, float z) :
	_v0(_mm_setr_ps(x, 0.0f, 0.0f, 0.0f)),
	_v1(_mm_setr_ps(0.0f, y, 0.0f, 0.0f)),
	_v2(_mm_setr_ps(0.0f, 0.0f, z, 0.0f)),
	_v3(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f)) {};

Matrix::Matrix(const Matrix &t)
	: _v0(t._v0), _v1(t._v1), _v2(t._v2), _v3(t._v3) {};

Matrix::Matrix(const Vect &tV0, const Vect &tV1, const Vect &tV2, const Vect &tV3)
	: _v0(tV0), _v1(tV1), _v2(tV2), _v3(tV3) {};

//TODO:: check to see if identity or zero is more common and make that the default also can add additonal types of special matrices
Matrix::Matrix(MatrixSpecialType t)
	: _v0(_mm_set_ps1(0.0f)), _v1(_mm_set_ps1(0.0f)), _v2(_mm_set_ps1(0.0f)), _v3(_mm_set_ps1(0.0f))
{
	switch (t)
	{
	case ZERO:
		break;
	case IDENTITY:
		_m0 = 1.0f;
		_m5 = 1.0f;
		_m10 = 1.0f;
		_m15 = 1.0f;
		break;
	default:
		assert(false);
		break;
	}
};

Matrix::Matrix(RotType type, float radians) :
	_v0(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f)),
	_v1(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f)),
	_v2(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f)),
	_v3(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f)) //set to identity first
{

	float c = cosf(radians);
	float s = sinf(radians);

	switch (type)
	{
	case ROT_X:
		_m5 = c;
		_m6 = s;
		_m9 = -s;
		_m10 = c;
		break;
	case ROT_Y:
		_m0 = c;
		_m2 = -s;
		_m8 = s;
		_m10 = c;
		break;
	case ROT_Z:
		_m0 = c;
		_m1 = s;
		_m4 = -s;
		_m5 = c;
		break;
	default:
		assert(false);
		break;
	}
};

//Accessors (getters amd setters)

float& Matrix::operator[] (m0_enum) { return _m0; }
float& Matrix::operator[] (m1_enum) { return _m1; }
float& Matrix::operator[] (m2_enum) { return _m2; }
float& Matrix::operator[] (m3_enum) { return _m3; }
float& Matrix::operator[] (m4_enum) { return _m4; }
float& Matrix::operator[] (m5_enum) { return _m5; }
float& Matrix::operator[] (m6_enum) { return _m6; }
float& Matrix::operator[] (m7_enum) { return _m7; }
float& Matrix::operator[] (m8_enum) { return _m8; }
float& Matrix::operator[] (m9_enum) { return _m9; }
float& Matrix::operator[] (m10_enum) { return _m10; }
float& Matrix::operator[] (m11_enum) { return _m11; }
float& Matrix::operator[] (m12_enum) { return _m12; }
float& Matrix::operator[] (m13_enum) { return _m13; }
float& Matrix::operator[] (m14_enum) { return _m14; }
float& Matrix::operator[] (m15_enum) { return _m15; }

float  Matrix::operator[] (m0_enum)  const { return _m0; }
float  Matrix::operator[] (m1_enum)  const { return _m1; }
float  Matrix::operator[] (m2_enum)  const { return _m2; }
float  Matrix::operator[] (m3_enum)  const { return _m3; }
float  Matrix::operator[] (m4_enum)  const { return _m4; }
float  Matrix::operator[] (m5_enum)  const { return _m5; }
float  Matrix::operator[] (m6_enum)  const { return _m6; }
float  Matrix::operator[] (m7_enum)  const { return _m7; }
float  Matrix::operator[] (m8_enum)  const { return _m8; }
float  Matrix::operator[] (m9_enum)  const { return _m9; }
float  Matrix::operator[] (m10_enum) const { return _m10; }
float  Matrix::operator[] (m11_enum) const { return _m11; }
float  Matrix::operator[] (m12_enum) const { return _m12; }
float  Matrix::operator[] (m13_enum) const { return _m13; }
float  Matrix::operator[] (m14_enum) const { return _m14; }
float  Matrix::operator[] (m15_enum) const { return _m15; }

float& Matrix::m0() { return _m0; }
float& Matrix::m1() { return _m1; }
float& Matrix::m2() { return _m2; }
float& Matrix::m3() { return _m3; }
float& Matrix::m4() { return _m4; }
float& Matrix::m5() { return _m5; }
float& Matrix::m6() { return _m6; }
float& Matrix::m7() { return _m7; }
float& Matrix::m8() { return _m8; }
float& Matrix::m9() { return _m9; }
float& Matrix::m10() { return _m10; }
float& Matrix::m11() { return _m11; }
float& Matrix::m12() { return _m12; }
float& Matrix::m13() { return _m13; }
float& Matrix::m14() { return _m14; }
float& Matrix::m15() { return _m15; }

float Matrix::m0()  const { return _m0; }
float Matrix::m1()  const { return _m1; }
float Matrix::m2()  const { return _m2; }
float Matrix::m3()  const { return _m3; }
float Matrix::m4()  const { return _m4; }
float Matrix::m5()  const { return _m5; }
float Matrix::m6()  const { return _m6; }
float Matrix::m7()  const { return _m7; }
float Matrix::m8()  const { return _m8; }
float Matrix::m9()  const { return _m9; }
float Matrix::m10() const { return _m10; }
float Matrix::m11() const { return _m11; }
float Matrix::m12() const { return _m12; }
float Matrix::m13() const { return _m13; }
float Matrix::m14() const { return _m14; }
float Matrix::m15() const { return _m15; }

Vect Matrix::v0() const { return _v0; }
Vect Matrix::v1() const { return _v1; }
Vect Matrix::v2() const { return _v2; }
Vect Matrix::v3() const { return _v3; }

Vect Matrix::get(MatrixRowType row)
{
	Vect retval;
	switch (row)
	{
	case ROW_0:
		retval = _v0;
		break;
	case ROW_1:
		retval = _v1;
		break;
	case ROW_2:
		retval = _v2;
		break;
	case ROW_3:
		retval = _v3;
		break;
	default:
		break;
	}
	return retval;
};

void Matrix::set(Vect v0, Vect v1, Vect v2, Vect v3)
{
	_v0.set(v0);
	_v1.set(v1);
	_v2.set(v2);
	_v3.set(v3);
};

//TODO: check the performance impact of set_ps1 vs set_ps
void Matrix::set(MatrixSpecialType t)
{
	_v0.set(_mm_set_ps1(0.0f));
	_v1.set(_mm_set_ps1(0.0f));
	_v2.set(_mm_set_ps1(0.0f));
	_v3.set(_mm_set_ps1(0.0f));

	switch (t)
	{
	case ZERO:
		break;
	case IDENTITY:
		_m0 = 1.0f;
		_m5 = 1.0f;
		_m10 = 1.0f;
		_m15 = 1.0f;
		break;
	default:
		break;
	}
};

void Matrix::set(MatrixRowType row, Vect inV)
{
	switch (row)
	{
	case MatrixRowType::ROW_0:
		_v0.set(inV);
		break;
	case MatrixRowType::ROW_1:
		_v1.set(inV);
		break;
	case MatrixRowType::ROW_2:
		_v2.set(inV);
		break;
	case MatrixRowType::ROW_3:
		_v3.set(inV);
		break;
	default:
		assert(false);
		break;
	}
}

void Matrix::set(MatrixTransType, float x, float y, float z)
{
	_v0.set(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f));
	_v1.set(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f));
	_v2.set(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));
	_v3.set(_mm_setr_ps(x, y, z, 1.0f));
}
//TODO: check the performance differnce between set and setr
void Matrix::set(MatrixScaleType, float x, float y, float z)
{
	_v0.set(_mm_setr_ps(x, 0.0f, 0.0f, 0.0f));
	_v1.set(_mm_setr_ps(0.0f, y, 0.0f, 0.0f));
	_v2.set(_mm_setr_ps(0.0f, 0.0f, z, 0.0f));
	_v3.set(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
}

void Matrix::set(MatrixScaleType, Vect v)
{
	_v0.set(_mm_setr_ps(v.x(), 0.0f, 0.0f, 0.0f));
	_v1.set(_mm_setr_ps(0.0f, v.y(), 0.0f, 0.0f));
	_v2.set(_mm_setr_ps(0.0f, 0.0f, v.z(), 0.0f));
	_v3.set(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
}

void Matrix::set(MatrixTransType, Vect v)
{
	_v0.set(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f));
	_v1.set(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f));
	_v2.set(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));
	_v3.set(v);
}

void Matrix::set(RotType type, float f)
{
	_v0.set(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f));
	_v1.set(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f));
	_v2.set(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));
	_v3.set(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));//set to identity first

	float c = cosf(f);
	float s = sinf(f);

	switch (type)
	{
	case ROT_X:
		_m5 = c;
		_m6 = s;
		_m9 = -s;
		_m10 = c;
		break;
	case ROT_Y:
		_m0 = c;
		_m2 = -s;
		_m8 = s;
		_m10 = c;
		break;
	case ROT_Z:
		_m0 = c;
		_m1 = s;
		_m4 = -s;
		_m5 = c;
		break;
	default:
		assert(false);
		break;
	}
}

//Operators

Matrix& Matrix::operator = (const Matrix& m)
{

	_v0 = m._v0;
	_v1 = m._v1;
	_v2 = m._v2;
	_v3 = m._v3;
	return *this;
}

Matrix Matrix::operator + ()
{
	return Matrix(*this);
};

Matrix Matrix::operator - ()
{
	return Matrix(_v0.getNegate(), _v1.getNegate(), _v2.getNegate(), _v3.getNegate());
};

Matrix& Matrix::operator += (const Matrix& m)
{
	_v0.mVectorAddE(m._v0);
	_v1.mVectorAddE(m._v1);
	_v2.mVectorAddE(m._v2);
	_v3.mVectorAddE(m._v3);
	return *this;
};

Matrix& Matrix::operator -= (const Matrix& m)
{
	_v0.mVectorSubE(m._v0);
	_v1.mVectorSubE(m._v1);
	_v2.mVectorSubE(m._v2);
	_v3.mVectorSubE(m._v3);
	return *this;
};

Matrix Matrix::operator - (const Matrix& m)
{
	return Matrix(
		_v0.mVectorSub(m._v0),
		_v1.mVectorSub(m._v1),
		_v2.mVectorSub(m._v2),
		_v3.mVectorSub(m._v3));
};

Matrix Matrix::operator + (const Matrix& m)
{
	return Matrix(
		_v0.mVectorAdd(m._v0),
		_v1.mVectorAdd(m._v1),
		_v2.mVectorAdd(m._v2),
		_v3.mVectorAdd(m._v3));
};

Vect Matrix::operator * (const Vect &v)
{
	return v*  (*this);
}

Matrix& Matrix::operator *= (const float& scaleFactor)
{
	_v0.mVectorMulE(scaleFactor);
	_v1.mVectorMulE(scaleFactor);
	_v2.mVectorMulE(scaleFactor);
	_v3.mVectorMulE(scaleFactor);
	return *this;
}

Matrix Matrix::operator * (const float& scaleFactor) const
{
	return Matrix
	(
		Vect(_v0.mVectorMul(scaleFactor)),
		Vect(_v1.mVectorMul(scaleFactor)),
		Vect(_v2.mVectorMul(scaleFactor)),
		Vect(_v3.mVectorMul(scaleFactor))
	);
}

Matrix Matrix::operator * (const Matrix& m2)
{
	return Matrix(
		_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v0.x()), m2._v0), _mm_mul_ps(_mm_set_ps1(this->_v0.y()), m2._v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v0.z()), m2._v2), _mm_mul_ps(_mm_set_ps1(this->_v0.w()), m2._v3))),
		_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v1.x()), m2._v0), _mm_mul_ps(_mm_set_ps1(this->_v1.y()), m2._v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v1.z()), m2._v2), _mm_mul_ps(_mm_set_ps1(this->_v1.w()), m2._v3))),
		_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v2.x()), m2._v0), _mm_mul_ps(_mm_set_ps1(this->_v2.y()), m2._v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v2.z()), m2._v2), _mm_mul_ps(_mm_set_ps1(this->_v2.w()), m2._v3))),
		_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v3.x()), m2._v0), _mm_mul_ps(_mm_set_ps1(this->_v3.y()), m2._v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v3.z()), m2._v2), _mm_mul_ps(_mm_set_ps1(this->_v3.w()), m2._v3)))
	);

	//return Matrix(
	//	_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v0.x()), this->_v0), _mm_mul_ps(_mm_set_ps1(m2._v0.y()), this->_v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v0.z()), this->_v2), _mm_mul_ps(_mm_set_ps1(m2._v0.w()), this->_v3))),
	//	_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v1.x()), this->_v0), _mm_mul_ps(_mm_set_ps1(m2._v1.y()), this->_v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v1.z()), this->_v2), _mm_mul_ps(_mm_set_ps1(m2._v1.w()), this->_v3))),
	//	_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v2.x()), this->_v0), _mm_mul_ps(_mm_set_ps1(m2._v2.y()), this->_v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v2.z()), this->_v2), _mm_mul_ps(_mm_set_ps1(m2._v2.w()), this->_v3))),
	//	_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v3.x()), this->_v0), _mm_mul_ps(_mm_set_ps1(m2._v3.y()), this->_v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v3.z()), this->_v2), _mm_mul_ps(_mm_set_ps1(m2._v3.w()), this->_v3)))
	//);
}

Matrix Matrix::operator *= (const Matrix& m2)
{
	this->set
	(
		_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v0.x()), m2._v0), _mm_mul_ps(_mm_set_ps1(this->_v0.y()), m2._v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v0.z()), m2._v2), _mm_mul_ps(_mm_set_ps1(this->_v0.w()), m2._v3))),
		_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v1.x()), m2._v0), _mm_mul_ps(_mm_set_ps1(this->_v1.y()), m2._v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v1.z()), m2._v2), _mm_mul_ps(_mm_set_ps1(this->_v1.w()), m2._v3))),
		_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v2.x()), m2._v0), _mm_mul_ps(_mm_set_ps1(this->_v2.y()), m2._v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v2.z()), m2._v2), _mm_mul_ps(_mm_set_ps1(this->_v2.w()), m2._v3))),
		_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v3.x()), m2._v0), _mm_mul_ps(_mm_set_ps1(this->_v3.y()), m2._v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(this->_v3.z()), m2._v2), _mm_mul_ps(_mm_set_ps1(this->_v3.w()), m2._v3)))
	);

	/*this->set
	(
	_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v0.x()), this->_v0), _mm_mul_ps(_mm_set_ps1(m2._v0.y()), this->_v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v0.z()), this->_v2), _mm_mul_ps(_mm_set_ps1(m2._v0.w()), this->_v3))),
	_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v1.x()), this->_v0), _mm_mul_ps(_mm_set_ps1(m2._v1.y()), this->_v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v1.z()), this->_v2), _mm_mul_ps(_mm_set_ps1(m2._v1.w()), this->_v3))),
	_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v2.x()), this->_v0), _mm_mul_ps(_mm_set_ps1(m2._v2.y()), this->_v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v2.z()), this->_v2), _mm_mul_ps(_mm_set_ps1(m2._v2.w()), this->_v3))),
	_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v3.x()), this->_v0), _mm_mul_ps(_mm_set_ps1(m2._v3.y()), this->_v1)), _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m2._v3.z()), this->_v2), _mm_mul_ps(_mm_set_ps1(m2._v3.w()), this->_v3)))
	);*/

	return *this;
};

//Methods

//TODO: optimize and simd everything below this line, especially the comparison functions
void Matrix::inv()
{
	*this = (1 / det() * this->adj());
}

Matrix Matrix::getInv()
{
	return 1 / det() * this->adj();
}

Matrix Matrix::getT() const
{
	__m128 v0(_v0);
	__m128 v1(_v1);
	__m128 v2(_v2);
	__m128 v3(_v3);

	_MM_TRANSPOSE4_PS(v0, v1, v2, v3)
		return Matrix(v0, v1, v2, v3);
}

//todo SIMD and optimze
float Matrix::det()
{
	//layout for sanitys sake while setting this up
	//m0  m1  m2  m3
	//m4  m5  m6  m7
	//m8  m9  m10 m11
	//m12 m13 m14 m15

	float det1 = _m0 * det3x3(_m5, _m6, _m7, _m9, _m10, _m11, _m13, _m14, _m15);
	float det2 = _m1 * -det3x3(_m4, _m6, _m7, _m8, _m10, _m11, _m12, _m14, _m15);
	float det3 = _m2 * det3x3(_m4, _m5, _m7, _m8, _m9, _m11, _m12, _m13, _m15);
	float det4 = _m3 * -det3x3(_m4, _m5, _m6, _m8, _m9, _m10, _m12, _m13, _m14);

	return det1 + det2 + det3 + det4;
}

void Matrix::T()
{
	_MM_TRANSPOSE4_PS(_v0, _v1, _v2, _v3)
}

bool Matrix::isIdentity(float tolerance)
{
	bool retval = false;

	if (identityV0(tolerance))
	{
		if (identityV1(tolerance))
		{
			if (identityV2(tolerance))
			{
				if (identityV3(tolerance))
				{
					retval = true;
				}
			}
		}
	}

	return retval;
}

bool Matrix::isIdentity()
{
	bool retval = false;

	if (identityV0(MATH_TOLERANCE))
	{
		if (identityV1(MATH_TOLERANCE))
		{
			if (identityV2(MATH_TOLERANCE))
			{
				if (identityV3(MATH_TOLERANCE))
				{
					retval = true;
				}
			}
		}
	}

	return retval;
}

bool Matrix::isEqual(const Matrix& rhs)
{
	bool retVal = false;

	if (_v0.isEqual(rhs._v0))
	{
		if (_v1.isEqual(rhs._v1))
		{
			if (_v2.isEqual(rhs._v2))
			{
				if (_v3.isEqual(rhs._v3))
				{
					retVal = true;
				}
			}
		}
	}

	return retVal;
}
//private functions

//TODO:: optimize theese especially the utility checks I need to do them with SIMD instructions ASAP
//TODO: Optimize
Matrix Matrix::adj()
{

	//layout for sanitys sake while setting this up
	//m0  m1  m2  m3
	//m4  m5  m6  m7
	//m8  m9  m10 m11
	//m12 m13 m14 m15
	Matrix C;

	//first row
	C._m0 = det3x3(_m5, _m6, _m7, _m9, _m10, _m11, _m13, _m14, _m15);
	C._m1 = -det3x3(_m4, _m6, _m7, _m8, _m10, _m11, _m12, _m14, _m15);
	C._m2 = det3x3(_m4, _m5, _m7, _m8, _m9, _m11, _m12, _m13, _m15);
	C._m3 = -det3x3(_m4, _m5, _m6, _m8, _m9, _m10, _m12, _m13, _m14);

	//second row 
	C._m4 = -det3x3(_m1, _m2, _m3, _m9, _m10, _m11, _m13, _m14, _m15);
	C._m5 = det3x3(_m0, _m2, _m3, _m8, _m10, _m11, _m12, _m14, _m15);
	C._m6 = -det3x3(_m0, _m1, _m3, _m8, _m9, _m11, _m12, _m13, _m15);
	C._m7 = det3x3(_m0, _m1, _m2, _m8, _m9, _m10, _m12, _m13, _m14);

	//third row  
	C._m8 = det3x3(_m1, _m2, _m3, _m5, _m6, _m7, _m13, _m14, _m15);
	C._m9 = -det3x3(_m0, _m2, _m3, _m4, _m6, _m7, _m12, _m14, _m15);
	C._m10 = det3x3(_m0, _m1, _m3, _m4, _m5, _m7, _m12, _m13, _m15);
	C._m11 = -det3x3(_m0, _m1, _m2, _m4, _m5, _m6, _m12, _m13, _m14);

	//fourth row 
	C._m12 = -det3x3(_m1, _m2, _m3, _m5, _m6, _m7, _m9, _m10, _m11);
	C._m13 = det3x3(_m0, _m2, _m3, _m4, _m6, _m7, _m8, _m10, _m11);
	C._m14 = -det3x3(_m0, _m1, _m3, _m4, _m5, _m7, _m8, _m9, _m11);
	C._m15 = det3x3(_m0, _m1, _m2, _m4, _m5, _m6, _m8, _m9, _m10);

	C.T();
	return C;
}


float Matrix::det3x3(
	float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	float det1 = m0 * det2x2(m4, m5, m7, m8);
	float det2 = m1 * -det2x2(m3, m5, m6, m8);
	float det3 = m2 * det2x2(m3, m4, m6, m7);

	return det1 + det2 + det3;
}

float Matrix::det2x2(float m0, float m1, float m2, float m3)
{
	return (m0*m3) - (m1*m2);
}

bool Matrix::identityV0(const float& tolerance)
{
	bool retval = false;
	if (Util::isOne(_m0, tolerance))
	{
		if (Util::isZero(_m1, tolerance))
		{
			if (Util::isZero(_m2, tolerance))
			{
				if (Util::isZero(_m3, tolerance))
				{
					retval = true;
				}
			}
		}
	}
	return retval;
}

bool Matrix::identityV1(const float& tolerance)
{
	bool retval = false;
	if (Util::isZero(_m4, tolerance))
	{
		if (Util::isOne(_m5, tolerance))
		{
			if (Util::isZero(_m6, tolerance))
			{
				if (Util::isZero(_m7, tolerance))
				{
					retval = true;
				}
			}
		}
	}
	return retval;
}

bool Matrix::identityV2(const float& tolerance)
{
	bool retval = false;
	if (Util::isZero(_m8, tolerance))
	{
		if (Util::isZero(_m9, tolerance))
		{
			if (Util::isOne(_m10, tolerance))
			{
				if (Util::isZero(_m11, tolerance))
				{
					retval = true;
				}
			}
		}
	}
	return retval;
}

bool Matrix::identityV3(const float& tolerance)
{
	bool retval = false;
	if (Util::isZero(_m12, tolerance))
	{
		if (Util::isZero(_m13, tolerance))
		{
			if (Util::isZero(_m14, tolerance))
			{
				if (Util::isOne(_m15, tolerance))
				{
					retval = true;
				}
			}
		}
	}
	return retval;
}


// ---  End of File ---------------
