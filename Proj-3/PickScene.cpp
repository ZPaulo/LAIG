#include "PickScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"

#include "ExampleObject.h"

#include <math.h>

#include "CGFappearance.h"

#define NUM_OBJS 7
#define NUM_ROWS 5
#define NUM_COLS 4

void PickScene::init() 
{
	// normal init, no changes needed

	// Enables lighting computations
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	// Sets up some lighting parameters
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient);  // Define ambient light

	float pos[4] = {2.5,7,2.5,1};

	light0 = new CGFlight(GL_LIGHT0, pos);


	// Defines a default normal
	glNormal3f(0,0,1);

	//elements = new Game(5);

}

void PickScene::setSel(float pos[4],bool on)
{
	
	lightSel = new CGFlight(GL_LIGHT1, pos);
	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,50);
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,10);
	float dir[3] = {0,-1,0};
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,dir);

	if(on)
		lightSel->enable();
	else
		lightSel->disable();
	lightSel->update();
}

void PickScene::display() 
{

	// ---- BEGIN Background, camera and axis setup

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();

	// Draw (and update) light
	light0->enable();
	light0->draw();

	// Draw axis
	axis.draw();


	// ---- END Background, camera and axis setup



	// scale down a bit	
	glScalef(0.4, 0.4, 0.4);

	// picking example, the important parts are the gl*Name functions
	// and the code in the associted PickInterface class

	elements->init();

	// ---- END feature demos

	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

PickScene::~PickScene()
{
	delete(light0);
	delete(materialAppearance);
	delete(lightSel);
	delete(elements);
}
