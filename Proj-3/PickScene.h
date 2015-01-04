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
	bool nextTurn,start;
	int difficulty,versus;
	~PickScene();
private:
	Game *elements;
	float wA, inc,startAng;
	unsigned long t0;


	CGFappearance* materialAppearance;
};

#endif