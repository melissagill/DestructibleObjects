/*
// Melissa Gill
// March 2014
//
// Class definition of a 3x3 matrix class with
// basic functionality added as needed
*/

#pragma once
#include"Vector3.h"
#include<math.h>

enum AngleMode
{
	Degrees,
	Radians
};

class Matrix3
{
	public:
		Vector3 matrix[3];

		Matrix3(){matrix[0]=Vector3();matrix[1]=Vector3();matrix[2]=Vector3();};
		Matrix3(Vector3 r1,Vector3 r2,Vector3 r3){matrix[0]=r1;matrix[1]=r2;matrix[2]=r3;};

		float get(int row,int col);
		void set(int row,int col,float value);

		static Matrix3 Identity();
		static Matrix3 RotationX(float rotation,AngleMode mode);
		static Matrix3 RotationY(float rotation,AngleMode mode);
		static Matrix3 RotationZ(float rotation,AngleMode mode);

		Matrix3 operator*(Matrix3 rhs);
		Vector3 operator*(Vector3 rhs);
};