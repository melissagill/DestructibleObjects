/*
// Melissa Gill
// March 2014
*/

#include"Matrix3.h"

float Matrix3::get(int row,int col)
{
	return matrix[row].get(col);
}

void Matrix3::set(int row,int col,float value)
{
	matrix[row].set(col,value);
}

Matrix3 Matrix3::Identity()
{
	return Matrix3(Vector3(1,0,0),
			       Vector3(0,1,0),
				   Vector3(0,0,1));
}

Matrix3 Matrix3::RotationX(float rotation,AngleMode mode)
{
	float angle=rotation;
	if(mode==Degrees)
		angle*=3.14159265f/180;
	Matrix3 rot(Vector3(1,0,0),
				Vector3(0,cos(angle),-sin(angle)),
				Vector3(0,sin(angle),cos(angle)));
	return rot;
}

Matrix3 Matrix3::RotationY(float rotation,AngleMode mode)
{
	float angle=rotation;
	if(mode==Degrees)
		angle*=3.14159265f/180;
	Matrix3 rot(Vector3(cos(angle),0,sin(angle)),
				Vector3(0,1,0),
				Vector3(-sin(angle),0,cos(angle)));
	return rot;
}

Matrix3 Matrix3::RotationZ(float rotation,AngleMode mode)
{
	float angle=rotation;
	if(mode==Degrees)
		angle*=3.14159265f/180;
	Matrix3 rot(Vector3(cos(angle),-sin(angle),0),
				Vector3(sin(angle),cos(angle),0),
				Vector3(0,0,1));
	return rot;
}

Matrix3 Matrix3::operator*(Matrix3 rhs)
{
	Matrix3 result;
	for(int i=0;i<3;i++)//row #
	{
		for(int k=0;k<3;k++)//col#
		{
			float value=0;
			for(int j=0;j<3;j++)//iter
			{
				value+=matrix[i].get(j)*rhs.matrix[j].get(k);
			}
			result.matrix[i].set(k,value);
		}
	}
	return result;
}

Vector3 Matrix3::operator*(Vector3 rhs)
{
	Vector3 result;
	for(int i=0;i<3;i++)//row #
	{
		float value=0;
		for(int j=0;j<3;j++)//col #
		{
			value+=matrix[i].get(j)*rhs.get(j);
		}
		result.set(i,value);
	}
	return result;
}