/*
// Melissa Gill
// March 2014
*/

#include"VoxelUnit.h"
#include"main.h"

float VoxelUnit::RegenTime=7;
bool VoxelUnit::RandomRegen=false;
float VoxelUnit::DeltaTime=.15f;

VoxelUnit::VoxelUnit(Vector3 position,Vector3 dimensions,float voxelSize,Vector3 color,bool ellipsoid)
	:Unit(position,Vector3(),Vector3(1,1,1),0,color)
{
	mDimensions=dimensions;
	mVoxelSize=voxelSize;
	
	//find how many voxels per bounding box edge
	mVoxelsDim=mDimensions/mVoxelSize;
	//make sure there are no partial voxels
	mVoxelsDim=Vector3((float)(int)mVoxelsDim.x,(float)(int)mVoxelsDim.y,(float)(int)mVoxelsDim.z);
	mDimensions=mVoxelsDim*mVoxelSize;
	
	//populate voxel array
	int voxels=0;
	mVoxels=new Voxel**[(int)mVoxelsDim.x];
	for(int x=0;x<mVoxelsDim.x;x++)
	{
		mVoxels[x]=new Voxel*[(int)mVoxelsDim.y];
		for(int y=0;y<mVoxelsDim.y;y++)
		{
			mVoxels[x][y]=new Voxel[(int)mVoxelsDim.z];
			for(int z=0;z<mVoxelsDim.z;z++)
			{
				voxels++;
				mVoxels[x][y][z].depth=CENTER;
				mVoxels[x][y][z].regenTimer=0;

				if(ellipsoid)//if ellipsoid check if voxel is within the ellisoid
				{
					Vector3 pos=mDimensions/-2;
					Vector3 index((float)x,(float)y,(float)z);
					pos+=index*mVoxelSize;
					Vector3 radius=mDimensions/2;
					Vector3 check;
					check.x=pos.x/radius.x;
					check.x*=check.x;
					check.y=pos.y/radius.y;
					check.y*=check.y;
					check.z=pos.z/radius.z;
					check.z*=check.z;
					if(check.x+check.y+check.z>1)//if not inside, set to never regenerate
						mVoxels[x][y][z].depth=EMPTY;
				}
			}
		}
	}

	//assign initial depth
	reassignDepth();

	cout<<"Voxels created: "<<voxels<<endl;
}

VoxelUnit::~VoxelUnit()
{
	for(int x=0;x<mVoxelsDim.x;x++)
	{
		for(int y=0;y<mVoxelsDim.y;y++)
		{
			delete [] mVoxels[x][y];
		}
		delete [] mVoxels[x];
	}
	delete [] mVoxels;
}

void VoxelUnit::update()
{
	bool reassign=false; //if nothing changes, we don't need to reassign depths
	for(int x=0;x<mVoxelsDim.x;x++)
	{
		for(int y=0;y<mVoxelsDim.y;y++)
		{
			for(int z=0;z<mVoxelsDim.z;z++)
			{
				if(mVoxels[x][y][z].depth==BORDER) //only update border voxel timers
				{
					mVoxels[x][y][z].regenTimer+=DeltaTime;
					//voxel fills back in if it has waited enough time
					if(mVoxels[x][y][z].regenTimer>=RegenTime)
					{
						mVoxels[x][y][z].depth=CENTER;
						mVoxels[x][y][z].regenTimer=0;
						reassign=true; //voxel has changed depth, so others need to be reassigned
					}
				}
			}
		}
	}
	if(reassign) //if something has changed, reassign depths
		reassignDepth();
}

void VoxelUnit::draw()
{
	for(int x=0;x<mVoxelsDim.x;x++)
	{
		for(int y=0;y<mVoxelsDim.y;y++)
		{
			for(int z=0;z<mVoxelsDim.z;z++)
			{
				if(mVoxels[x][y][z].depth==SURFACE) //only draw if on the surface
				{
					//calculate the voxel's position
					Vector3 pos=mPosition-(mVoxelsDim*mVoxelSize/2);
					Vector3 index((float)x,(float)y,(float)z);
					pos+=index*mVoxelSize;

					//change the color for a gradient effect, for demo visibility
					Vector3 color=(Vector3(1,1,1)-mColor);
					color.x*=index.x/mVoxelsDim.x;
					color.y*=index.y/mVoxelsDim.y;
					color.z*=index.z/mVoxelsDim.z;

					glPushMatrix();//only apply position and color to this voxel

					glColor3f(color.x,color.y,color.z);
					glTranslatef(pos.x,pos.y,pos.z);

					glutSolidCube(mVoxelSize);//draw voxel

					glPopMatrix();
				}
			}
		}
	}
}

bool VoxelUnit::checkCollision(Vector3 center,float radius,int checkDepth)
{
	bool colliding=false;

	//calculate the bottom left and top right of the bounding box
	Vector3 bottomLeft=mPosition-(mVoxelsDim*mVoxelSize/2);
	Vector3 topRight=mPosition+(mVoxelsDim*mVoxelSize/2);

	//if the sphere doesn't collide with the bounding box, stop
	if(!checkBoxCollision(topRight,bottomLeft,center,radius))
		return false;

	for(int x=0;x<mVoxelsDim.x;x++)
	{
		for(int y=0;y<mVoxelsDim.y;y++)
		{
			for(int z=0;z<mVoxelsDim.z;z++)
			{
				if(mVoxels[x][y][z].depth<0) //don't bother checking already destroyed voxels
					continue;
				if(mVoxels[x][y][z].depth!=checkDepth && checkDepth!=INT_MAX) //in case a specific depth was specified for checking
					continue;

				//find voxel position
				Vector3 index((float)x,(float)y,(float)z);
				Vector3 pos=mPosition-(mVoxelsDim*mVoxelSize/2);
				pos+=index*mVoxelSize;
				//find limits of voxel
				bottomLeft=pos-(Vector3(1,1,1)*mVoxelSize/2);
				topRight=pos+(Vector3(1,1,1)*mVoxelSize/2);

				//check collision on voxel
				if(checkBoxCollision(topRight,bottomLeft,center,radius))
				{
					//if colliding, destroy voxel
					mVoxels[x][y][z].depth=DESTROYED;
					mVoxels[x][y][z].regenTimer=0;
					colliding=true;
				}
			}
		}
	}
	if(colliding) //if collision happened, need to reassign voxel depth values
		reassignDepth();
	return colliding;
}

bool VoxelUnit::checkBoxCollision(Vector3 topRight,Vector3 bottomLeft,Vector3 center,float radius)
{
	//basic box to sphere collision based on pythagorean
	float sqR=radius*radius;

	if(center.x<bottomLeft.x)
		sqR-=(center.x-bottomLeft.x)*(center.x-bottomLeft.x);
	else if(center.x>topRight.x)
		sqR-=(center.x-topRight.x)*(center.x-topRight.x);

	if(center.y<bottomLeft.y)
		sqR-=(center.y-bottomLeft.y)*(center.y-bottomLeft.y);
	else if(center.y>topRight.y)
		sqR-=(center.y-topRight.y)*(center.y-topRight.y);

	if(center.z<bottomLeft.z)
		sqR-=(center.z-bottomLeft.z)*(center.z-bottomLeft.z);
	else if(center.z>topRight.z)
		sqR-=(center.z-topRight.z)*(center.z-topRight.z);
	
	return sqR>0;
}

void VoxelUnit::reassignDepth()
{
	for(int x=0;x<mVoxelsDim.x;x++)
	{
		for(int y=0;y<mVoxelsDim.y;y++)
		{
			for(int z=0;z<mVoxelsDim.z;z++)
			{
				Vector3 index((float)x,(float)y,(float)z);

				//get adjacent voxels, there will always be six
				Voxel* adj=getAdjacent(index);

				if(mVoxels[x][y][z].depth>=0)//if this is a full voxel
				{
					bool outsidePresent=false;
					for(int i=0;i<6;i++)
					{
						if(adj[i].depth<0)//if any adjacent voxels are not full
							outsidePresent=true;
					}
					if(outsidePresent)
						mVoxels[x][y][z].depth=SURFACE; //if touching empty voxels, must be on surface
					else
						mVoxels[x][y][z].depth=CENTER; //otherwise it's in the center
				}
				else if(mVoxels[x][y][z].depth!=EMPTY)//if a not full voxel, and not one that is permanently empty
				{
					bool objectPresent=false;
					for(int i=0;i<6;i++)
					{
						if(adj[i].depth>=0)//if any adjacent voxels are full
							objectPresent=true;
					}
					if(objectPresent)
					{
						mVoxels[x][y][z].depth=BORDER;//if touching a full voxel, must be on the border
						//if(RandomRegen)
							//mVoxels[x][y][z].regenTimer=(float)(rand()%(int)RegenTime);
					}
					else
						mVoxels[x][y][z].depth=DESTROYED;//otherwise it's just destroyed
				}
			}
		}
	}
}

Voxel* VoxelUnit::getAdjacent(Vector3 index)
{
	//get the six adjacent voxels
	Voxel adj[6];

	//populate with empty voxels
		//ensures that boundary voxels will have six adjacent voxels
	for(int i=0;i<6;i++)
	{
		adj[i].depth=EMPTY;
		adj[i].regenTimer=0;
	}

	if(index.x-1>=0)
		adj[0].depth=mVoxels[(int)index.x-1][(int)index.y][(int)index.z].depth;
	if(index.x+1<mVoxelsDim.x)
		adj[1].depth=mVoxels[(int)index.x+1][(int)index.y][(int)index.z].depth;
	if(index.y-1>=0)
		adj[2].depth=mVoxels[(int)index.x][(int)index.y-1][(int)index.z].depth;
	if(index.y+1<mVoxelsDim.y)
		adj[3].depth=mVoxels[(int)index.x][(int)index.y+1][(int)index.z].depth;
	if(index.z-1>=0)
		adj[4].depth=mVoxels[(int)index.x][(int)index.y][(int)index.z-1].depth;
	if(index.z+1<mVoxelsDim.z)
		adj[5].depth=mVoxels[(int)index.x][(int)index.y][(int)index.z+1].depth;

	return adj;
}