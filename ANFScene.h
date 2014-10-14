#ifndef _ANFSCENE_H_
#define _ANFSCENE_H_

#include "tinyxml.h"
#include "Parser.h"
#include "CGFscene.h"
#include "CGFappearance.h"
#include "CGFapplication.h"

class ANFScene : public CGFscene
{
public:
	Parser parser;
	ANFScene(char *filename);
	~ANFScene();

	void init();
	void display();

	void drawGraph(string nodeID);

	vector<CGFlight*> lights;
	vector<CGFappearance*> appearances;

	
protected:

	TiXmlDocument* doc;

	TiXmlElement* globalsElement; 
	TiXmlElement* lightsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* textureElement;
	TiXmlElement* graphElement;
};

#endif