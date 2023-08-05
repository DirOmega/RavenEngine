#ifndef ENGINE_MATH_VECT_H
#define ENGINE_MATH_VECT_H

class Matrix; //forward declare

#include <xmmintrin.h>
#include <smmintrin.h> 

class alignas(16) Vect 
{
	
public:

	Vect();

	Vect(const Vect &t);

	Vect(const __m128& m);

	//sets W to 1
	Vect(const __m128& m, VectW1); //users shouldn't need theese I may make them private later but I'm leaving them open in case a user needs em
	//sets W to 0
	Vect(const __m128& m, VectW0); //users shouldn't need theese I may make them private later but I'm leaving them open in case a user needs em

	Vect(const float &tx, const float &ty, const float &tz, const float &tw);

	Vect(const float &tx, const float &ty, const float &tz);

	~Vect();

	//in the origonal code this is set backwards with setr_ps
	void set(const float &tx, const float& ty, const float &tz);

	//in the origonal code this is set backwards with setr_ps
	void set(const float &tx, const float& ty, const float &tz, const float &tw);

	void set(const Vect& c);
	
	void set(const __m128& m);

	void set(const __m128& m, const VectW0&);

	void set(const __m128& m, const VectW1&);

	float mag() const;

	float magSqr() const;

	//TODO: optimzie theese
	bool isZero() const;

	bool isZero(float tolerance) const;

	bool isEqual(const Vect& rhs) const;

	bool isEqual(const Vect& rhs, float tolerance) const;

	float& operator[] (x_enum);
	float& operator[] (y_enum);
	float& operator[] (z_enum);
	float& operator[] (w_enum);

	float operator[] (x_enum) const;
	float operator[] (y_enum) const;
	float operator[] (z_enum) const;
	float operator[] (w_enum) const;

	float x() const;
	float y() const;
	float z() const;
	float w() const;

	float& x();
	float& y();
	float& z();
	float& w();

	Vect& operator = (const Vect&tmp);

	//TODO:: benchmark the static cas vs the constructor overhead
	Vect operator - () const;

	Vect operator + () const;

	Vect operator + (const float& f) const;

	Vect& operator += (const Vect &tmp);

	Vect& operator -= (const Vect &tmp);

	Vect operator - (const Vect& tmp) const;

	Vect operator + (const Vect& tmp) const;

	Vect operator / (const Vect &tmp) const;

	Vect& operator /= (const Vect &tmp);

	Vect& operator *= (const float& s);
	
	Vect operator * (const float& s) const;

	Vect& operator *= (const Matrix&);

	Vect operator * (const Matrix&) const; 

	operator __m128() const;

	Vect cross(const Vect& rhs) const;

	float dot(const Vect&tmp) const;

	void norm();
	
	Vect getNorm() const;

	float getAngle(const Vect& v2);

	//Matrix Support Functions
	Vect getNegate();

	Vect& mVectorAddE(const Vect& rhs);

	Vect& mVectorSubE(const Vect& rhs);
	
	Vect mVectorAdd(const Vect& rhs) const;

	Vect mVectorSub(const Vect& rhs) const;

	Vect& mVectorMulE(const float& s);

	Vect mVectorMul(const float& s) const;

	friend Vect operator * (const float& f, const Vect& v)
	{
		return v * f;
	}

private:

	union
	{
		__m128	_m;

		// anonymous struct
		struct
		{
			float _vx;
			float _vy;
			float _vz;
			float _vw;
		};
	};

};


#endif

// ---  End of File ---------------
