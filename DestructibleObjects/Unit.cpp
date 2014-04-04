/*
// Melissa Gill
// March 2014
*/

#include"Unit.h"
#include"main.h"

Unit::Unit(Vector3 position,Vector3 rotation,Vector3 scale,float radius,Vector3 color)
{
	mPosition=position;
	mRotation=rotation;
	mScale=scale;
	mRadius=radius;
	mColor=color;
}

void Unit::draw()
{
	glPushMatrix(); //apply these transformations to only this unit

	glTranslatef((float)mPosition.x,(float)mPosition.y,(float)mPosition.z);
	glRotatef((float)mRotation.y,0,1,0);
	glRotatef((float)mRotation.x,1,0,0);
	glScalef((float)mScale.x,(float)mScale.y,(float)mScale.z);
	
	//uncomment if you want the surface of the sphere rendered as well
	//glColor3f((float)mColor.x,(float)mColor.y,(float)mColor.z);
	//glutSolidSphere(mRadius,20,20);
	glColor3f((float)mColor.x/1.5f,(float)mColor.y/1.5f,(float)mColor.z/1.5f);
	glutWireSphere(mRadius,10,10);

	glPopMatrix();
}

void Unit::move(Vector3 offset)
{
	mPosition+=offset;
}

void Unit::rotate(Vector3 rot)
{
	mRotation+=rot;
}