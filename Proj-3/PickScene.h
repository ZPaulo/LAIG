#ifndef PICKSCENE_H
#define PICKSCENE_H

#include "CGFscene.h"
#include "PickInterface.h"
#include "Game.h"

class PickScene : public CGFscene
{
	friend PickInterface;
public:
	void init();
	void display();
	void setSel(float pos[4],bool on);
	~PickScene();
private:
	CGFlight *light0;
	CGFlight *lightSel;
	Game *elements;
	CGFappearance* materialAppearance;
};

#endif