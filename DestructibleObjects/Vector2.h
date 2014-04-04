/*
// Melissa Gill
// March 2014
//
// Class definition of a Vector2 with basic functionality
// added as needed
*/

#pragma once

class Vector2
{
	public:
		float x;
		float y;

		Vector2(){x=0;y=0;};
		Vector2(float pX,float pY){x=pX;y=pY;};

		float Magnitude(){return sqrt((x*x)+(y*y));};
		float MagnitudeSquared(){return (x*x)+(y*y);};

		Vector2 operator-(Vector2 rhs){return Vector2(x-rhs.x,y-rhs.y);};
		Vector2 operator/(float rhs){return Vector2(x/rhs,y/rhs);};
		Vector2& operator+=(const Vector2 &rhs){x+=rhs.x;y+=rhs.y;return *this;};
};