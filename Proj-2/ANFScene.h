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

	void drawGraph(string nodeID,string app);
	
	vector<CGFlight*> lightsV;
	void activateLight(int id,bool enable);
	
protected:

	TiXmlDocument* doc;

	TiXmlElement* globalsElement; 
	TiXmlElement* lightsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* textureElement;
	TiXmlElement* animationElement;
	TiXmlElement* graphElement;
};


#endif