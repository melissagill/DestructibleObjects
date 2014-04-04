/*
// Melissa Gill
// March 2014
*/

#include"Game.h"
#include"main.h"
#include<cstring>
#include<string>
#include<fstream>
#include<sstream>

float Game::DeformationRadius=2.3f;
bool Game::DisplayControls=false;
bool Game::ShowTargetPoint=false;

void Game::init()
{
	mCameraPos=Vector3(0,0,-10);
	mCameraRot=Vector3(0,0,0);
	
	//create voxel units
	VoxelUnit* voxel=new VoxelUnit(Vector3(0,-2,0),//position
		Vector3(6,5,4),//dimensions
		.2f,//voxel size
		Vector3((float)(rand()%50)/100,(float)(rand()%50)/100,(float)(rand()%50)/100),//base color
		false);//not an ellipsoid
	mVoxels.push_back(voxel);

	voxel=new VoxelUnit(Vector3(0,2,0),//position
		Vector3(7,3,5),//dimensions
		.1f,//voxel size
		Vector3((float)(rand()%100)/100,(float)(rand()%100)/100,(float)(rand()%100)/100),//base color
		true);//make an ellipsoid
	mVoxels.push_back(voxel);

	//set the position of our aim unit
	mAim=new Unit(Vector3(0,0,0),Vector3(),Vector3(1,1,1),1,Vector3(1,1,1));
	mAim->setPos(getPositionRelativeToCamera(Vector3(0,0,10)));
}

void Game::cleanup()
{
	for(int i=0;i<(int)mVoxels.size();i++)
		delete mVoxels[i];
	for(int i=0;i<(int)mSpheres.size();i++)
		delete mSpheres[i];
	mVoxels.clear();
	mSpheres.clear();
	delete mAim;
}

void Game::update()
{
	//update all voxelUnits
	for(int i=0;i<(int)mVoxels.size();i++)
	{
		mVoxels[i]->update(); //update voxelUnit, for regeneration purposes
		for(int j=0;j<(int)mSpheres.size();j++)
			mVoxels[i]->checkCollision(mSpheres[j]->getPos(),mSpheres[j]->getRadius(),SURFACE);
			//check collision with each sphere on screen (aside from aim)
	}
	mAim->setRadius(DeformationRadius);
}

void Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	//Camera Setup
	glPushMatrix();

	glTranslatef(mCameraPos.x,mCameraPos.y,mCameraPos.z);

	glRotatef(mCameraRot.x,1.0f,0.0f,0.0f);
	glRotatef(mCameraRot.y,0.0f,1.0f,0.0f);
	
	for(int i=0;i<(int)mVoxels.size();i++)//draw all voxelUnits
		mVoxels[i]->draw();

	for(int i=0;i<(int)mSpheres.size();i++)//draw all spheres
		mSpheres[i]->draw();

	if(ShowTargetPoint)//toggleable aim sphere
		mAim->draw();
	
	Vector3 pos(.4f,-.35f,1);
	drawLabel("f: show/hide controls",getPositionRelativeToCamera(pos),Vector3(1,1,1));
	if(DisplayControls) //toggleable controls
	{
		float line=.05f;pos.y+=line;
		drawLabel("z: toggle display aim point mesh",getPositionRelativeToCamera(pos),Vector3(1,1,1));
		pos.y+=line;
		drawLabel("Camera: wasdqe - movement",getPositionRelativeToCamera(pos),Vector3(1,1,1));
		pos.y+=line;
		drawLabel("Camera: ijkl   - rotation",getPositionRelativeToCamera(pos),Vector3(1,1,1));
		pos.y+=line;
		drawLabel("+/-: change deformation radius",getPositionRelativeToCamera(pos),Vector3(1,1,1));
		pos.y+=line;
		drawLabel("*/6: change regen time",getPositionRelativeToCamera(pos),Vector3(1,1,1));
		pos.y+=line;
		drawLabel("t: toggle place sphere",getPositionRelativeToCamera(pos),Vector3(1,1,1));
		pos.y+=line;
		drawLabel("space: destroy voxels",getPositionRelativeToCamera(pos),Vector3(1,1,1));
		pos.y+=line;
		drawLabel("r: reset scene",getPositionRelativeToCamera(pos),Vector3(1,1,1));
		pos.y+=line;
		drawLabel("c: remove permanent spheres",getPositionRelativeToCamera(pos),Vector3(1,1,1));
		pos.y+=line;
		drawLabel("Esc to exit",getPositionRelativeToCamera(pos),Vector3(1,1,1));
	}

	glPopMatrix();
	
	glutSwapBuffers();
}

void Game::drawLabel(string word,Vector3 pos,Vector3 color)
{
	//convert string to a char*
	char* buffer=new char[word.length()+1];
	strcpy_s(buffer,word.length()+1, word.c_str());

	//set up color and position
	glColor4f(color.x,color.y,color.z,1.0f);
	glRasterPos3f(pos.x,pos.y,pos.z);

	//draw each character
	int len = (int)strlen(buffer);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
}

void Game::moveCamera(Vector3 dir)
{
	mCameraPos+=dir;
	mAim->setPos(getPositionRelativeToCamera(Vector3(0,0,10)));
}

void Game::rotateCamera(Vector3 rot)
{
	mCameraRot+=rot;
	mAim->setPos(getPositionRelativeToCamera(Vector3(0,0,10)));
}

void Game::poke()
{
	//check every voxelUnit and remove voxels within the deformation radius
	for(int i=0;i<(int)mVoxels.size();i++)
		mVoxels[i]->checkCollision(mAim->getPos(),DeformationRadius);
}

void Game::placeSphere()
{
	//make a new sphere a the position of aim and radius of DeformationRadius
	Unit* unit=new Unit(mAim->getPos(), //position
			Vector3(), //rotation=(0,0,0)
			Vector3(1,1,1), //scale
			DeformationRadius, //radius
			Vector3((float)(rand()%100)/100,(float)(rand()%100)/100,(float)(rand()%100)/100));//color
	mSpheres.push_back(unit);
}

void Game::removeSpheres()
{
	for(int i=0;i<(int)mSpheres.size();i++)
		delete mSpheres[i];
	mSpheres.clear();
}

//for getting positions that stay directly in front of the camera by an offset
Vector3 Game::getPositionRelativeToCamera(Vector3 offset)
{
	//set position to be transformed
	Vector3 pos=mCameraPos*-1;
	pos-=offset;

	//get the rotation matrix and multiply it by the position vector
	Matrix3 rot=Matrix3::RotationY(-mCameraRot.y,Degrees)*Matrix3::RotationX(-mCameraRot.x,Degrees);
	pos=rot*pos;

	return pos;
}