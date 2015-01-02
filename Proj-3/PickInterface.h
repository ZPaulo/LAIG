#ifndef PickInterface_H
#define PickInterface_H

#include "CGFinterface.h"

class PickInterface: public CGFinterface {
	public:
		virtual void processMouse(int button, int state, int x, int y);	
		void performPicking(int x, int y, bool disk);
		void processHits(GLint hits, GLuint buffer[], bool disk); 
};


#endif
