#ifndef _ANFSCENE_H_
#define _ANFSCENE_H_

#include "tinyxml.h"

class ANFScene
{
public:
	ANFScene(char *filename);
	~ANFScene();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	
protected:

	TiXmlDocument* doc;

	TiXmlElement* globalsElement; 
	TiXmlElement* lightsElement;
	TiXmlElement* camerasElement;
<<<<<<< HEAD
	TiXmlElement* appearancesElement;
=======
	TiXmlElement* textureElement;

>>>>>>> fad5900f3e23902573acb015bc986c773acaca32
};

#endif