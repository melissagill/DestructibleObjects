/*
// Melissa Gill
// March 2014
//
// VoxelUnit class definition, holds and manages Voxel
// structs.
// Extends Unit.
*/

#pragma once
#include"Unit.h"
#include<vector>

//used to define the "depth" of each voxel
enum Depth
{
	EMPTY=-3, //will never refill
	DESTROYED, //will refill eventually
	BORDER, //touching a full voxel, next to refill
	SURFACE, //touching an inactive voxel, first read for collision and only ones displayed
	CENTER //full, but not rendered because not on the surface
};

struct Voxel
{
	Depth depth;
	float regenTimer; //used when depth=BORDER; determines how long to wait to regen
};

class VoxelUnit:public Unit //extends unit for basic position/rotation values
{
	private:
		static float DeltaTime; // hard set frame time, could be reset by actual timer

		Vector3 mDimensions; //dimensions of the whole unit
		Vector3 mVoxelsDim; //how many voxels along each edge of the bounding box
		float mVoxelSize; //edge size of each voxel

		Voxel*** mVoxels; //array to hold all voxels

		void reassignDepth(); //reassign the depth value of each voxel
		Voxel* getAdjacent(Vector3 index); //get the adjacent voxels to the one at index
		bool checkBoxCollision(Vector3 topRight,Vector3 bottomLeft,Vector3 center,float radius);//check collision of a box with a sphere

	public:
		static bool RandomRegen; //boolean indicating whether regen is in clean layers or more random
		static float RegenTime; //voxels regenerate if they have been BORDER voxels for this amount of time

		VoxelUnit(Vector3 position,Vector3 dimensions,float voxelSize,Vector3 color=Vector3(.7f,.7f,.7f),bool ellipsoid=false);
		~VoxelUnit();

		void update();
		void draw();

		bool checkCollision(Vector3 center,float radius,int checkDepth=INT_MAX); //check voxel collision with a sphere

		Vector3 getDimensions(){return mDimensions;};
};