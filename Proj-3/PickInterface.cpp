#include "PickInterface.h"
#include "PickScene.h"
#include "CGFapplication.h"

// buffer to be used to store the hits during picking
#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];

void PickInterface::processMouse(int button, int state, int x, int y) 
{
	CGFinterface::processMouse(button,state, x, y);

	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y,true);
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y,false);
}

void PickInterface::performPicking(int x, int y, bool disk) 
{
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer (BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);

	// Initialize the picking name stack
	glInitNames();

	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);

	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix ();

	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,projmat);

	// reset projection matrix
	glLoadIdentity();

	// get current viewport and use it as reference for 
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// this is multiplied in the projection matrix
	gluPickMatrix ((GLdouble) x, (GLdouble) (CGFapplication::height - y), 5.0, 5.0, viewport);

	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);

	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer
	scene->display();

	// restore original projection matrix
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();

	glFlush();

	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf,disk);
}

void PickInterface::processHits (GLint hits, GLuint buffer[],bool disk) 
{
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected=NULL;
	GLuint nselected;

	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			selected = ptr;
			nselected=num;
		}
		for (int j=0; j < num; j++) 
			ptr++;
	}

	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected!=NULL)
	{
		// this should be replaced by code handling the picked object's ID's (stored in "selected"), 
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");
		for (int i=0; i<nselected; i++)
			printf("%d ",selected[i]);
		printf("\n");
		if(nselected == 2){
			float pos[4];
			pos[0] = ((PickScene *) scene)->elements->brd.coords[selected[0]][selected[1]][0]+2.5;
			pos[1] = ((PickScene *) scene)->elements->brd.coords[selected[0]][selected[1]][1];
			pos[2] = ((PickScene *) scene)->elements->brd.coords[selected[0]][selected[1]][2]+2.5;
			pos[3] = 1;
			printf("coord %f %f %f\n",pos[0],pos[1],pos[2]);
			((PickScene *) scene)->setSel(pos,true);

			int pl = ((PickScene *) scene)->elements->activePl;
			float oldP[2] ={((PickScene *) scene)->elements->brd.plIndex[pl][0],((PickScene *) scene)->elements->brd.plIndex[pl][1]};
			float newP[2] = {selected[0],selected[1]};

			bool valid = ((PickScene *) scene)->elements->calculateMove(oldP,newP,disk);

		}
		else
		{
			float pos[4] = {0,0,0,1};
			((PickScene *) scene)->setSel(pos,false);
		}

	}
	else{
		printf("Nothing selected while picking \n");	
		float pos[4] = {0,0,0,1};
		((PickScene *) scene)->setSel(pos,false);
	}
}


PickInterface::PickInterface()
{
}
void PickInterface::initGUI()
{
	GLUI_Panel *geral =addPanel("Opcoes", 1);
	addColumnToPanel(geral);
	GLUI_Panel *jogadaPanel = addPanelToPanel(geral,"Movimentos", 1);
	addButtonToPanel(jogadaPanel,"Undo",9);
	addButtonToPanel(jogadaPanel,"End Turn",10);
	addColumnToPanel(geral);
	GLUI_Panel *luzesPanel = addPanelToPanel(geral,"Luzes", 1);

	for(unsigned int i=0;i<((PickScene *) scene)->elements->lights.size();i++){

		string str=((PickScene *) scene)->elements->lights[i]->id;
		char * writable = new char[str.size() + 1];
		copy(str.begin(), str.end(), writable);
		writable[str.size()] = '\0';


		if(((PickScene *) scene)->elements->lights[i]->enabled==true){
			addCheckboxToPanel(luzesPanel,writable,NULL,i)->set_int_val(1);
		}
		else{
			addCheckboxToPanel(luzesPanel,writable,NULL,i)->set_int_val(0);
		}
		delete[] writable;

	}

	addColumnToPanel(geral);
	GLUI_Panel *camerasPanel = addPanelToPanel(geral,"Camaras", 1);
	GLUI_RadioGroup *cameraList = addRadioGroupToPanel(camerasPanel,&(((PickScene *) scene)->elements->activeCam));

	for(unsigned int i=0;i<((PickScene *) scene)->elements->cams.size();i++){

		string str=((PickScene *) scene)->elements->cams[i]->id;
		char * writable = new char[str.size() + 1];
		copy(str.begin(), str.end(), writable);
		writable[str.size()] = '\0';

		if(i == ((PickScene *) scene)->elements->activeCam)
			addRadioButtonToGroup(cameraList, writable)->set_int_val(1);
		else
			addRadioButtonToGroup(cameraList, writable);

		delete[] writable;

	}
	addRadioButtonToGroup(cameraList, "Default");

	addColumnToPanel(geral);
	GLUI_Panel *drawPanel = addPanelToPanel(geral,"Draw Mode", 1);
	GLUI_RadioGroup *drawList = addRadioGroupToPanel(drawPanel,&(((PickScene *) scene)->drawingMode));
	addRadioButtonToGroup(drawList, "Fill");
	addRadioButtonToGroup(drawList, "Line");
	addRadioButtonToGroup(drawList, "Point");



}

void PickInterface::processGUI(GLUI_Control *ctrl)
{
	if(ctrl->user_id >=0 && ctrl->user_id < 8)
		if( ctrl->get_int_val() == 1)
			((PickScene *) scene)->elements->lights[ctrl->user_id]->enabled = true;
		else 
			((PickScene *) scene)->elements->lights[ctrl->user_id]->enabled = false;
	else
		if(ctrl->user_id == 10){
			if(((PickScene *) scene)->elements->validMove()){
				((PickScene *) scene)->nextTurn = true;
				((PickScene *) scene)->t0 = 0;
				int *pl = &((PickScene *) scene)->elements->activePl;

				if(*pl == 0)
					*pl = 1;
				else
					*pl = 0;
			}

		}
		else if(ctrl->user_id == 9){
			((PickScene *) scene)->elements->undo();
		}

}