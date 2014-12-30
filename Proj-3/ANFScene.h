#ifndef _ANFSCENE_H_
#define _ANFSCENE_H_

#include "tinyxml.h"
#include "CGFscene.h"
#include "ExampleObject.h"
#include "Parser.h"
#include "Game.h"

class ANFScene : public CGFscene
{
public:
	Parser parser;
	Game elements;
	
	CGFobject* obj;
	ANFScene(char *filename);
	~ANFScene();

	void init();
	void display();
	void update(unsigned long t);

	void drawGraph(string nodeID,string app,bool init);
	void createDisplayList(string nodeID,string app);

	vector<CGFlight*> lightsV;
	void activateLight(int id,bool enable);

	void resetAnimations();
	
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