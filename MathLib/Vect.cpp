
#include <xmmintrin.h>
#include <smmintrin.h> 
#include <math.h>
#include "Matrix.h"

#include "Constants.h"
#include "Util.h"
//Constructors

Vect::Vect() : _m(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f)) {};

Vect::Vect(const Vect &t) : _m(t._m) {};

Vect::Vect(const __m128& m) : _m(m) {};

//maybe make the two below private?
//Also consider moving the second step into the consturctor I'm worried that the compiler may change ordering in release mode
Vect::Vect(const __m128& m, VectW1) : _m(m)
{
	_vw = 1.0f;
};

Vect::Vect(const __m128& m, VectW0) : _m(m)
{
	_vw = 0.0f;
}
Vect::Vect(const float &tx, const float &ty, const float &tz, const float &tw) 
	: _m(_mm_setr_ps(tx, ty, tz, tw)) {};

Vect::Vect(const float &tx, const float &ty, const float &tz) : _m(_mm_setr_ps(tx, ty, tz, 1.0f)) {};

Vect::~Vect() {};


//Accessors

float& Vect::operator[] (x_enum) { return _vx; };
float& Vect::operator[] (y_enum) { return _vy; };
float& Vect::operator[] (z_enum) { return _vz; };
float& Vect::operator[] (w_enum) { return _vw; };

float Vect::operator[] (x_enum) const { return _vx; };
float Vect::operator[] (y_enum) const { return _vy; };
float Vect::operator[] (z_enum) const { return _vz; };
float Vect::operator[] (w_enum) const { return _vw; };

float Vect::x() const { return _vx; };
float Vect::y() const { return _vy; };
float Vect::z() const { return _vz; };
float Vect::w() const { return _vw; };

float& Vect::x() { return _vx; };
float& Vect::y() { return _vy; };
float& Vect::z() { return _vz; };
float& Vect::w() { return _vw; };

void Vect::set(const float &tx, const float& ty, const float &tz)
{
	_m = _mm_setr_ps(tx, ty, tz, 1.0f);
}

void Vect::set(const float &tx, const float& ty, const float &tz, const float &tw)
{
	_m = _mm_setr_ps(tx, ty, tz, tw);
}

void Vect::set(const Vect& c)
{
	this->_m = c._m;
}

void Vect::set(const __m128& m)
{
	this->_m = m;
}

void Vect::set(const __m128& m, const VectW0&)
{
	this->_m = m;
	_vw = 0.0f;
}

void Vect::set(const __m128& m, const VectW1&)
{
	this->_m = m;
	_vw = 1.0f;
}

//Operators

Vect& Vect::operator = (const Vect&tmp)
{
	_m = tmp._m;
	return *this;
}

Vect Vect::operator - () const
{
	return Vect(_mm_setr_ps(-_vx, -_vy, -_vz, 1.0f));
};

Vect Vect::operator + () const
{
	return Vect(_mm_setr_ps(_vx, _vy, _vz, 1.0f));
};

Vect Vect::operator + (const float& f) const
{
	return static_cast<Vect>(_mm_add_ps(_m, _mm_set_ps1(f)));
}

Vect& Vect::operator += (const Vect &tmp)
{
	this->set(_mm_add_ps(_m, tmp._m), w1);
	return *this;
};

Vect& Vect::operator -= (const Vect &tmp)
{
	this->set(_mm_sub_ps(_m, tmp._m), w1);
	return *this;
};

Vect Vect::operator - (const Vect& tmp) const
{
	return Vect(_mm_sub_ps(_m, tmp._m), w1);
};

Vect Vect::operator + (const Vect& tmp) const
{
	return Vect(_mm_add_ps(_m, tmp._m), w1);
};

Vect Vect::operator / (const Vect &tmp) const
{
	return Vect(_mm_div_ps(_m, tmp._m), w1);
};

Vect& Vect::operator /= (const Vect &tmp)
{
	set(_mm_div_ps(_m, tmp._m), w1);
	return *this;
}

Vect& Vect::operator *= (const float& s)
{
	set(_mm_mul_ps(_m, _mm_set_ps1(s)), w1);
	return *this;
};

Vect Vect::operator * (const float& s) const
{
	return Vect(_mm_mul_ps(_m, _mm_set_ps1(s)), w1);
}

Vect& Vect::operator *= (const Matrix& M)
{
	_m = _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(_vx), M.v0()._m), _mm_mul_ps(_mm_set_ps1(_vy), M.v1()._m)), _mm_add_ps((_mm_mul_ps(_mm_set_ps1(_vz), M.v2()._m)), _mm_mul_ps(_mm_set_ps1(_vw), M.v3()._m)));
	return *this;
}

Vect Vect::operator * (const Matrix& M) const
{
	return Vect(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set_ps1(_vx), M.v0()._m), _mm_mul_ps(_mm_set_ps1(_vy), M.v1()._m)), _mm_add_ps((_mm_mul_ps(_mm_set_ps1(_vz), M.v2()._m)), _mm_mul_ps(_mm_set_ps1(_vw), M.v3()._m))));
}

Vect::operator __m128() const
{
	return _m;
}

// Methods



float Vect::mag() const
{
	Vect tmp(_m, VectW0::w0);
	__m128 t = _mm_mul_ps(tmp._m, tmp._m);
	t = _mm_hadd_ps(t, t);
	tmp.set(_mm_sqrt_ps(_mm_hadd_ps(t, t)));
	return tmp._vx;
}

float Vect::magSqr() const
{
	Vect tmp(_m, VectW0::w0);
	__m128 t = _mm_mul_ps(tmp._m, tmp._m);
	t = _mm_hadd_ps(t, t);
	tmp.set(_mm_hadd_ps(t, t));
	return tmp._vx;
}

bool Vect::isZero() const
{
	bool retval = false;
	if (Util::isZero(_vx, MATH_TOLERANCE))
	{
		if (Util::isZero(_vy, MATH_TOLERANCE))
		{
			if (Util::isZero(_vz, MATH_TOLERANCE))
			{
				if (Util::isZero(_vw - 1.0f, MATH_TOLERANCE))
				{
					retval = true;
				}
			}
		}
	}
	return retval;
}

bool Vect::isZero(float tolerance) const
{
	bool retval = false;
	if (Util::isZero(_vx, tolerance))
	{
		if (Util::isZero(_vy, tolerance))
		{
			if (Util::isZero(_vz, tolerance))
			{
				if (Util::isZero(_vw - 1.0f, tolerance))
				{
					retval = true;
				}
			}
		}
	}
	return retval;
}

bool Vect::isEqual(const Vect& rhs) const
{
	bool retval = false;
	if (Util::isEqual(_vx, rhs._vx, MATH_TOLERANCE))
	{
		if (Util::isEqual(_vy, rhs._vy, MATH_TOLERANCE))
		{
			if (Util::isEqual(_vz, rhs._vz, MATH_TOLERANCE))
			{
				if (Util::isEqual(_vw, rhs._vw, MATH_TOLERANCE))
				{
					retval = true;
				}
			}
		}
	}
	return retval;
}

bool Vect::isEqual(const Vect& rhs, float tolerance) const
{
	bool retval = false;
	if (Util::isEqual(_vx, rhs._vx, tolerance))
	{
		if (Util::isEqual(_vy, rhs._vy, tolerance))
		{
			if (Util::isEqual(_vz, rhs._vz, tolerance))
			{
				if (Util::isEqual(_vw, rhs._vw, tolerance))
				{
					retval = true;
				}
			}
		}
	}
	return retval;
}

Vect Vect::cross(const Vect& rhs) const
{
	//woot woot 1 line cross product that I spent way to much time on
	return Vect(
		_mm_sub_ps(
			_mm_mul_ps(
				_mm_shuffle_ps(_m, _m, _MM_SHUFFLE(0, 0, 2, 1)),
				_mm_shuffle_ps(rhs._m, rhs._m, _MM_SHUFFLE(0, 1, 0, 2))
			),
			_mm_mul_ps(
				_mm_shuffle_ps(_m, _m, _MM_SHUFFLE(0, 1, 0, 2)),
				_mm_shuffle_ps(rhs._m, rhs._m, _MM_SHUFFLE(0, 0, 2, 1))
			)
		)
		, w1);
}

float Vect::dot(const Vect&tmp) const
{
	//which is better? this
	//Vect4D_SIMD t = Vect4D_SIMD(_mm_mul_ps(m, tmp.m));
	//return(t.x + t.y + t.z + t.w);	

	//or this?
	//__m128 t = _mm_mul_ps(_m, tmp._m);
	//t = _mm_hadd_ps(t, t);
	//t = _mm_hadd_ps(t, t);
	//float f[4];
	//_mm_store_ss(f, t);
	//return f[0]-1;//-1 eleminates the extra 1

	//or this?
	Vect v(_mm_mul_ps(_m, tmp._m), w0);
	__m128 t = _mm_hadd_ps(v._m, v._m);
	v.set(_mm_hadd_ps(t, t));
	return v._vx;
};

void Vect::norm()
{
	this->set(_mm_div_ps(_m, _mm_set_ps1(this->mag())), w1);
};

Vect Vect::getNorm() const
{
	return Vect(_mm_div_ps(_m, _mm_set_ps1(this->mag())), w1);
};

float Vect::getAngle(const Vect& v2)
{
	return acosf(this->dot(v2) / (this->mag() * v2.mag()));
};

//Matrix Support Functions
Vect Vect::getNegate()
{
	return Vect(_mm_xor_ps(_m, _mm_set_ps1(-0.0f))); //this is a cheater way to flip the sign bit for all elements in the vector
};

Vect& Vect::mVectorAddE(const Vect& rhs)
{
	_m = _mm_add_ps(_m, rhs._m);
	return *this;
};

Vect& Vect::mVectorSubE(const Vect& rhs)
{
	_m = _mm_sub_ps(_m, rhs._m);
	return *this;
};

Vect Vect::mVectorAdd(const Vect& rhs) const
{
	return Vect(_mm_add_ps(_m, rhs._m));
};

Vect Vect::mVectorSub(const Vect& rhs) const
{
	return Vect(_mm_sub_ps(_m, rhs._m));
};

Vect& Vect::mVectorMulE(const float& s)
{
	set(_mm_mul_ps(_m, _mm_set_ps1(s)));
	return *this;
};

Vect Vect::mVectorMul(const float& s) const
{
	return Vect(_mm_mul_ps(_m, _mm_set_ps1(s)));
};

// ---  End of File ---------------
