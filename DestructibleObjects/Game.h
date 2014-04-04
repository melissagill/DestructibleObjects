/*
// Melissa Gill
// March 2014
//
// Game class: basically used as a scene class
// for managing all game objects, camera, and running
// input through.
*/

#pragma once
#include<stdlib.h>
#include<GL/glut.h>
#include<iostream>
#include<vector>
#include<math.h>

#include"Matrix3.h"
#include"Vector2.h"

#include"VoxelUnit.h"

using namespace std;

//manages objects in the scene
class Game
{
	private:
		//all spheres that have been placed in the scene
		vector<Unit*> mSpheres;

		//holder for all voxelUnits
		vector<VoxelUnit*> mVoxels;

		//unit to display where deformation will happen
		Unit* mAim;

		//camera position and rotation
		Vector3 mCameraPos;
		Vector3 mCameraRot;

		Vector3 getPositionRelativeToCamera(Vector3 offset); //for getting positions that stay directly in front of the camera by an offset

	public:
		//the radius within which voxels will be removed
			//accessed by main so it can be changed by input
		static float DeformationRadius;
		static bool DisplayControls;
		static bool ShowTargetPoint;

		Game(){};
		~Game(){};

		void init(); //set up initial scene values
		void cleanup(); //clean up scene objects

		void update(); //update all objects
		void draw(); //draw changes

		void drawLabel(string word,Vector3 pos,Vector3 color); //for drawing text

		//camera control functions
		void moveCamera(Vector3 dir);
		void rotateCamera(Vector3 rot);

		void poke(); //remove voxels within a radius of DeformationRadius
		void placeSphere(); //places a sphere with radis=DeformationRadius
		void removeSpheres(); //remove all placed spheres
};
