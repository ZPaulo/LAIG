#include "Interface.h"
#include "ANFScene.h"

Interface::Interface()
{
	testVar=0;
}
void Interface::initGUI()
{
	// Check CGFinterface.h and GLUI documentation for the types of controls available
	/*
	GLUI_Panel *varPanel= addPanel("Group", 1);
	addSpinnerToPanel(varPanel, "Val 1(interface)", 2, &testVar, 1);
	*/
	GLUI_Panel *geral =addPanel("Opcoes", 1);
	addColumnToPanel(geral);
	GLUI_Panel *luzesPanel = addPanelToPanel(geral,"Ativar Luzes", 1);
	int a=2;
	for(unsigned int i=0;i<((ANFScene *) scene)->parser.lights.size();i++){
		
		string str=((ANFScene *) scene)->parser.lights[i]->id;
		char * writable = new char[str.size() + 1];
		copy(str.begin(), str.end(), writable);
		writable[str.size()] = '\0';


		if(((ANFScene *) scene)->parser.lights[i]->enabled==true){
			addCheckboxToPanel(luzesPanel,writable,NULL,a+i)->set_int_val(1);
			printf("light %d enabled\n",i);
		}
		else{
			addCheckboxToPanel(luzesPanel,writable,NULL,a+i)->set_int_val(0);
			printf("light %d disabled\n",i);
		}
		delete[] writable;
	}

	/*addColumnToPanel(geral);
	GLUI_Panel *relogioPanel= addPanelToPanel(geral,"Relogio", 2);
	addCheckboxToPanel(relogioPanel,"Relogio",NULL,6)->set_int_val(1);

	GLUI_Listbox *listaTexturas = addListbox("Texturas",NULL,7);
	listaTexturas->add_item(8,"Simples");
	listaTexturas->add_item(9,"Madeira");
	listaTexturas->add_item(10,"Default");
	listaTexturas->set_int_val(10);
	*/


	// You could also pass a reference to a variable from the scene class, if public
	//	addSpinnerToPanel(varPanel, "Val 2(scene)", 2, &(((LightingScene*) scene)->sceneVar), 2);

}

void Interface::processGUI(GLUI_Control *ctrl)
{
	int a=2;
	if( ctrl->get_int_val() == 1){
		((ANFScene *) scene)->activateLight(ctrl->user_id-a,true);
			}
			else {
				((ANFScene *) scene)->activateLight(ctrl->user_id-a,false);
			}
	
}