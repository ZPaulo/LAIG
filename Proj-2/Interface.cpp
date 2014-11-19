#include "ANFScene.h"
#include "Interface.h"


Interface::Interface()
{
}
void Interface::initGUI()
{
	GLUI_Panel *geral =addPanel("Opcoes", 1);
	addColumnToPanel(geral);
	GLUI_Panel *luzesPanel = addPanelToPanel(geral,"Luzes", 1);

	for(unsigned int i=0;i<((ANFScene *) scene)->parser.lights.size();i++){

		string str=((ANFScene *) scene)->parser.lights[i]->id;
		char * writable = new char[str.size() + 1];
		copy(str.begin(), str.end(), writable);
		writable[str.size()] = '\0';


		if(((ANFScene *) scene)->parser.lights[i]->enabled==true){
			addCheckboxToPanel(luzesPanel,writable,NULL,i)->set_int_val(1);
		}
		else{
			addCheckboxToPanel(luzesPanel,writable,NULL,i)->set_int_val(0);
		}
		delete[] writable;

	}

	addColumnToPanel(geral);
	GLUI_Panel *camerasPanel = addPanelToPanel(geral,"Camaras", 1);
	GLUI_RadioGroup *cameraList = addRadioGroupToPanel(camerasPanel,&(((ANFScene *) scene)->parser.activeCam));
	
	for(unsigned int i=0;i<((ANFScene *) scene)->parser.cameras.size();i++){

		string str=((ANFScene *) scene)->parser.cameras[i]->id;
		char * writable = new char[str.size() + 1];
		copy(str.begin(), str.end(), writable);
		writable[str.size()] = '\0';

		if(i == ((ANFScene *) scene)->parser.activeCam)
			addRadioButtonToGroup(cameraList, writable)->set_int_val(1);
		else
			addRadioButtonToGroup(cameraList, writable);

		delete[] writable;

	}
	addRadioButtonToGroup(cameraList, "Default");

	addColumnToPanel(geral);
	GLUI_Panel *drawPanel = addPanelToPanel(geral,"Draw Mode", 1);
	GLUI_RadioGroup *drawList = addRadioGroupToPanel(drawPanel,&(((ANFScene *) scene)->parser.globals->drawing.mode));
	addRadioButtonToGroup(drawList, "Fill");
	addRadioButtonToGroup(drawList, "Line");
	addRadioButtonToGroup(drawList, "Point");
}

void Interface::processGUI(GLUI_Control *ctrl)
{
	if(ctrl->user_id >=0)
		if( ctrl->get_int_val() == 1)
			((ANFScene *) scene)->activateLight(ctrl->user_id,true);
		else 
			((ANFScene *) scene)->activateLight(ctrl->user_id,false);

}