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
	TiXmlElement* textureElement;
	TiXmlElement* graphElement;

};

#endif