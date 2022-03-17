//----------------------------------------------------------
// A 3d vector, Now with added dimensions!
// Bryn Stringer 
//----------------------------------------------------------
#pragma once
#include "math.h"

class util_vector3d
{
public:
	util_vector3d(void);
	util_vector3d(float,float,float);
	~util_vector3d(void);
	float x;
	float y;
	float z;
	util_vector3d ConvertRadianToVector(float p_Rad);

	util_vector3d operator +(util_vector3d input);
	util_vector3d operator -(util_vector3d input);
	util_vector3d operator *(float scale);

};
