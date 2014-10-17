#include "Interface.h"
#include "ANFScene.h"

Interface::Interface()
{
}
void Interface::initGUI()
{
	GLUI_Panel *geral =addPanel("Opcoes", 1);
	addColumnToPanel(geral);
	GLUI_Panel *luzesPanel = addPanelToPanel(geral,"Ativar Luzes", 1);
	//int a=2;
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
}

void Interface::processGUI(GLUI_Control *ctrl)
{
	int a=2;
	if( ctrl->get_int_val() == 1)
		((ANFScene *) scene)->activateLight(ctrl->user_id,true);
	else 
		((ANFScene *) scene)->activateLight(ctrl->user_id,false);


}