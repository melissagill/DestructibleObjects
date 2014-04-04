/*
// Melissa Gill
// March 2014
*/

#include"Vector3.h"
#include<math.h>

void Vector3::Normalize()
{
	float divisor=Magnitude();
	if(divisor==0)
		return;
	*this/=divisor;
}

float Vector3::Magnitude()
{
	return sqrt((x*x)+(y*y)+(z*z));
}

float Vector3::MagnitudeSquared()
{
	return ((x*x)+(y*y)+(z*z));
}

float Vector3::Dot(Vector3 rhs)
{
	return((x*rhs.x)+(y*rhs.y)+(z*rhs.z));
}

float Vector3::get(int index)
{
	switch (index)
	{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			return 0;
			break;
	}
}

void Vector3::set(int index,float value)
{
	switch (index)
	{
		case 0:
			x=value;
			break;
		case 1:
			y=value;
			break;
		case 2:
			z=value;
			break;
	}
}

Vector3& Vector3::operator=(const Vector3 &rhs)
{
	x=rhs.x; y=rhs.y; z=rhs.z;
	return *this;
}

Vector3& Vector3::operator+=(const Vector3 &rhs)
{
	x+=rhs.x; y+=rhs.y; z+=rhs.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3 &rhs)
{
	x-=rhs.x; y-=rhs.y; z-=rhs.z;
	return *this;
}

Vector3& Vector3::operator*=(const float &rhs)
{
	x*=rhs; y*=rhs; z*=rhs;
	return *this;
}

Vector3& Vector3::operator/=(const float &rhs)
{
	x/=rhs; y/=rhs; z/=rhs;
	return *this;
}

Vector3 Vector3::operator+(const Vector3 &rhs)
{
	Vector3 result(x+rhs.x,y+rhs.y,z+rhs.z);
	return result;
}

Vector3 Vector3::operator-(const Vector3 &rhs)
{
	Vector3 result(x-rhs.x,y-rhs.y,z-rhs.z);
	return result;
}

Vector3 Vector3::operator*(const float &rhs)
{
	Vector3 result(x*rhs,y*rhs,z*rhs);
	return result;
}

Vector3 Vector3::operator/(const float &rhs)
{
	Vector3 result(x/rhs,y/rhs,z/rhs);
	return result;
}

bool Vector3::operator==(const Vector3 &rhs)
{
	return (x==rhs.x && y==rhs.y && z==rhs.z);
}