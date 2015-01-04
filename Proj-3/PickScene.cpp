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


	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Defines a default normal
	glNormal3f(0,0,1);

	elements = new Game(5);

	drawingMode = 0;
	difficulty = 0;
	versus = 0;
	start = true;

	nextTurn = false;
	wA = 180;
	t0 = 0;
	inc = 0;
	startAng = 0;
}

void PickScene::setSel(float pos[4],bool on)
{

	elements->lights[1]->light = new CGFlight(GL_LIGHT1, pos);
	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,50);
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,10);
	float dir[3] = {0,-1,0};
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,dir);

	if(on)
		elements->lights[1]->light->enable();
	else
		elements->lights[1]->light->disable();
	elements->lights[1]->light->update();

	unsigned long updatePeriod=10;
	setUpdatePeriod(updatePeriod);

}

void PickScene::update(unsigned long t)
{
	elements->update(t);

	if (!nextTurn)
		return;
	if (t0 == 0)
		t0 = t;
	unsigned long dt = t - t0;
	if (dt < 500) {
		inc = (dt / 500.0) * wA;
	} else {
		nextTurn = false;
		startAng += 180;
		inc = 0;
	}


}

void PickScene::display() 
{

	// ---- BEGIN Background, camera and axis setup

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(elements->activeCam >= elements->cams.size())
		CGFscene::activeCamera->applyView();
	else{
		elements->cams[elements->activeCam]->apply();
	}

	CGFapplication::activeApp->forceRefresh();

	for(unsigned int i=0;i<elements->lights.size() && i<8;i++)
	{
		if(i != 1){
			if(elements->lights[i]->enabled)
				elements->lights[i]->light->enable();
			else
				elements->lights[i]->light->disable();
			elements->lights[i]->light->update();
		}
	}

	// Draw axis
	axis.draw();


	// ---- END Background, camera and axis setup

	if(drawingMode == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if(drawingMode == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	if(!start){

		glTranslatef(elements->size+0.5,0,elements->size+0.5);
		glRotatef(inc+startAng,0,1,0);
		glTranslatef(-(elements->size+0.5),0,-(elements->size+0.5));


		// scale down a bit	
		glScalef(0.4, 0.4, 0.4);
		glPushMatrix();
		glTranslatef(0,10,0);
		elements->plText->draw(elements->activePl);
		glPopMatrix();
		elements->brd.draw();
	}
	// ---- END feature demos

	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

PickScene::~PickScene()
{
	delete(materialAppearance);
	delete(elements);
}
