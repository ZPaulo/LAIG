#ifndef INTERFACE_H
#define INTERFACE_H

#include "CGFinterface.h"

class Interface: public CGFinterface {
	public:
		Interface();
		
		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);


};


#endif
