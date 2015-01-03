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
	void update(unsigned long t);
	void setSel(float pos[4],bool on);
	int drawingMode;
	~PickScene();
private:
	Game *elements;


	CGFappearance* materialAppearance;
};

#endif