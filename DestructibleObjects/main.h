/*
// Melissa Gill
// March 2014
*/

#pragma once
#include<stdlib.h>
#include<GL/glut.h>
#include<iostream>
#include<time.h>
#include"Vector2.h"

using namespace std;

//input functions registered through glut input
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);

//update scene function registered to glutIdleFunc
void update(void);

//basic camera information
extern Vector2 gWindowSize;
extern float gCameraAngle;
extern Vector2 gTopRightScreen;
extern Vector2 gBottomLeftScreen;

const float PI=3.14159265f;