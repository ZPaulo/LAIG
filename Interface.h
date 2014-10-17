#ifndef TPinterface_H
#define TPinterface_H

#include "CGFinterface.h"

class Interface: public CGFinterface {
	public:
		Interface();
		
		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);


		int testVar;
};


#endif
