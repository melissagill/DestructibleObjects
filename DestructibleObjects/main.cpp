/*
// Melissa Gill
// March 2014
*/

#include"main.h"
#include"Game.h"

//manages all scene object including all units and the camera
Game gGame;

//info for setting up the glut display
Vector2 gWindowSize=Vector2(700,700);
float gCameraAngle=45;
Vector2 gTopRightScreen=Vector2(1.0f*tan(gCameraAngle*PI/(360)),1.0f*tan(gCameraAngle*PI/(360)));
Vector2 gBottomLeftScreen=Vector2(1.0f*tan(gCameraAngle*-PI/(360)),1.0f*tan(gCameraAngle*-PI/(360)));

//some camera control info
float moveSpeed=.3f;
float speed=2.1f;

//input toggle between actually placing spheres and not
bool placeObj=false;

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));

	//initialize basic scene values
	gGame.init();

	//initialize glut display
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGBA| GLUT_DEPTH);
	glutInitWindowSize((int)gWindowSize.x,(int)gWindowSize.y);
	glutCreateWindow("GLUT Test");
	glClearColor(.15f,.15f,.15f,1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(gCameraAngle,gWindowSize.x/gWindowSize.y,1.0,1000.0);

	glMatrixMode(GL_MODELVIEW);

	glEnable (GL_DEPTH_TEST);
	
	//set up glut input functions (defined below)
	glutKeyboardFunc(&keyboard);
	glutKeyboardUpFunc(&keyboardUp);

	//assign our update loop
	glutIdleFunc(&update);

	glutMainLoop();

	//clean up all scene objects
	gGame.cleanup();

	return EXIT_SUCCESS;
}

//update and draw our scene, assigned to glutIdleFunc
void update(void)
{
	gGame.update();
	gGame.draw();
}

//basic controls, assigned to glutKeyboardFunc
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		//Esc to exit
		case '\x1B':
			exit(EXIT_SUCCESS);
			break;

		//basic camera movement
		case 'a':
			gGame.moveCamera(Vector3(moveSpeed,0,0));//right
			break;
		case 'd':
			gGame.moveCamera(Vector3(-moveSpeed,0,0));//left
			break;
		case 'w':
			gGame.moveCamera(Vector3(0,-moveSpeed,0));//up
			break;
		case 's':
			gGame.moveCamera(Vector3(0,moveSpeed,0));//down
			break;
		case 'q':
			gGame.moveCamera(Vector3(0,0,-moveSpeed));//forward
			break;
		case 'e':
			gGame.moveCamera(Vector3(0,0,moveSpeed));//back
			break;

		//basic camera rotation
		case 'j':
			gGame.rotateCamera(Vector3(0,speed,0));//left
			break;
		case 'l':
			gGame.rotateCamera(Vector3(0,-speed,0));//right
			break;
		case 'i':
			gGame.rotateCamera(Vector3(speed,0,0));//up
			break;
		case 'k':
			gGame.rotateCamera(Vector3(-speed,0,0));//down
			break;
			
		//change the size of the sphere to be removed from the voxel object
		case '+':
			Game::DeformationRadius+=.1f;
			break;
		case '-':
			Game::DeformationRadius-=.1f;
			if(Game::DeformationRadius<=0)
				Game::DeformationRadius=.1f;
			break;
			
		//change the amount of time it takes for voxels to regenerate
		case '*':
			VoxelUnit::RegenTime+=1;
			break;
		case '6':
			VoxelUnit::RegenTime-=1;
			if(VoxelUnit::RegenTime<=0)
				VoxelUnit::RegenTime=1;
			break;
	}
}

//on key up controls, assigned to glutKeyboardUpFunc
void keyboardUp(unsigned char key, int x, int y)
{
	switch(key)
	{
		//toggle display of the controls on-screen
		case 'f':
			Game::DisplayControls=!Game::DisplayControls;
			break;
		//toggle display of the location/radius of deformation
		case 'z':
			Game::ShowTargetPoint=!Game::ShowTargetPoint;
			break;
		//reset the scene
		case 'r':
			gGame.cleanup();
			gGame.init();
			break;
		//remove all spheres placed
		case 'c':
			gGame.removeSpheres();
			break;
		//space to either place a sphere or remove voxels within a radius
		//depending on placeObj
		case ' ':
			if(placeObj)
				gGame.placeSphere();
			else
				gGame.poke();
			break;
		//toggle placeObj
		case 't':
			placeObj=!placeObj;
			break;

		//display data about the current radius of voxel removal or regen time once after they've been changed
		case '+':
			cout<<"Deformation Radius: "<<Game::DeformationRadius<<endl;
			break;
		case '-':
			cout<<"Deformation Radius: "<<Game::DeformationRadius<<endl;
			break;
		case '*':
			cout<<"Regen Time: "<<VoxelUnit::RegenTime<<endl;
			break;
		case '6':
			cout<<"Regen Time: "<<VoxelUnit::RegenTime<<endl;
			break;
	}
}