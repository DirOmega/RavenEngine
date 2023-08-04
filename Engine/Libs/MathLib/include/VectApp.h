#pragma once

#include "Vect.h"

class VectApp
{
		
public:

	// lerp
	static void Lerp( Vect &out, const Vect &a, const Vect &b,  const float t );
	
	// lerp array
	static void LerpArray(Vect *out, const Vect *a, const Vect *b,   const float t, const int numVects );
	  						  

};

// ---  End of File ---------------
