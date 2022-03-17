//----------------------------------------------------------
// A 3d vector, Now with added dimensions!
// Bryn Stringer 
//----------------------------------------------------------
#include "util_vector3d.h"

util_vector3d::util_vector3d(void)
{
}

util_vector3d::util_vector3d(float xvalue, float yvalue, float zvalue)
{
	x = xvalue;
	y = yvalue;
	z = zvalue;
}

util_vector3d::~util_vector3d(void)
{
}

util_vector3d util_vector3d::ConvertRadianToVector(float p_Rad)
{
	//(0 here is 12 o'clock / North)
	util_vector3d temp(0,0,0);
	temp.x = sin(p_Rad);
	temp.z = cos(p_Rad);
	return temp;
}


util_vector3d util_vector3d::operator +(util_vector3d input)
{
	float tempx = x + input.x;
	float tempy = y + input.y;
	float tempz = z + input.z;
	util_vector3d temp(tempx,tempy,tempz);
	return temp;
}

util_vector3d util_vector3d::operator -(util_vector3d input)
{
	float tempx = x - input.x;
	float tempy = y - input.y;
	float tempz = z - input.z;
	util_vector3d temp(tempx,tempy,tempz);
	return temp;
}

util_vector3d util_vector3d::operator *(float scale)
{
	float tempx = x * scale;
	float tempy = y * scale;
	float tempz = z * scale;
	util_vector3d temp(tempx,tempy,tempz);
	return temp;
}