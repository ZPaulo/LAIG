#ifndef _PIECES
#define _PIECES

#include <String>
#include <vector>
#include <math.h>
#include "Parser.h"


class TowerP
{
public:
	Cylinder cil;
	CGFappearance* diskAppearance;
	bool moving;
	unsigned long t0;
	float vx,vy,vz;
	float pos[3], ori[3],dest[3];

	TowerP(){
		diskAppearance = new CGFappearance();
		t0 = 0;
		moving = false;
		cil.base = 2.5;
		cil.top = 2.5;
		cil.height = 0.5;
		cil.slices=20; 
		cil.stacks=20;
	}

	void update(unsigned long t){
		if (!moving)
			return;
		if (t0 == 0)
			t0 = t;
		unsigned long dt = t - t0;
		if (dt < 500) {
			pos[0] = ori[0] + (dt / 500.0) * vx;
			pos[1] = ori[1] + (dt / 500.0) * vy;
			pos[2] = ori[2] + (dt / 500.0) * vz;
		} else {
			moving = false;
			pos[0] = dest[0];
			pos[1] = dest[1];
			pos[2] = dest[2];
		}
	}

	void move(float x, float y, float z) {
		t0 = 0;
		moving = true;
		ori[0] = pos[0];
		ori[1] = pos[1];
		ori[2] = pos[2];
		vx = x - pos[0];
		vy = y - pos[1];
		vz = z - pos[2];
		dest[0] = x;
		dest[1] = y;
		dest[2] = z;
	}

	void draw(){
		glPushMatrix();
		glTranslatef(2.5+pos[0],(pos[1]+1)*0.5,2.5+pos[2]);
		glRotatef(90,1,0,0);
		cil.draw();
		glPopMatrix();
	}
	void draw(Texture *t){

		glPushMatrix();
		glTranslatef(2.5,0.5,2.5);
		glRotatef(90,1,0,0);
		cil.draw(t);
		glPopMatrix();
	}
};
class PlayerP
{
public:
	Cylinder cil;
	Sphere sp;
	int points;
	bool moving;
	unsigned long t0;
	float vx,vy,vz;
	float pos[3], ori[3],dest[3];

	PlayerP(){
		points = 0;
		t0 = 0;
		moving = false;
		cil.base = 1.5;
		cil.top = 0;
		cil.height = 1;
		cil.slices=20; 
		cil.stacks=20;

		sp.radius=1.5;
		sp.slices=20;
		sp.stacks=20;
	}

	void update(unsigned long t){
		if (!moving)
			return;
		if (t0 == 0)
			t0 = t;
		unsigned long dt = t - t0;
		if (dt < 500) {
			pos[0] = ori[0] + (dt / 500.0) * vx;
			pos[1] = ori[1] + (dt / 500.0) * vy;
			pos[2] = ori[2] + (dt / 500.0) * vz;
		} else {
			moving = false;
			pos[0] = dest[0];
			pos[1] = dest[1];
			pos[2] = dest[2];
		}
	}

	void move(float x, float y, float z) {
		t0 = 0;
		moving = true;
		ori[0] = pos[0];
		ori[1] = pos[1];
		ori[2] = pos[2];
		vx = x - pos[0];
		vy = y - pos[1];
		vz = z - pos[2];
		dest[0] = x;
		dest[1] = y;
		dest[2] = z;
	}


	void draw(){
		glPushMatrix();

		glTranslatef(pos[0]+2.5,(pos[1]+1)*0.5,pos[2]+2.5);

		glPushMatrix();
		glScaled(1,5,1);
		glRotatef(-90,1,0,0);
		cil.draw();
		glPopMatrix();


		//sp.draw();
		glPopMatrix();
	}
	void draw(Texture *t){
		glPushMatrix();
		//glTranslated(0,-1.,0);
		glScaled(1,5,1);
		cil.draw(t);
		glPopMatrix();

		sp.draw(t);
	}
};
class Cube
{
public:
	Cube(){}
	void draw(){
		Rectangle rec1;
		rec1.xy1[0] = 0; rec1.xy1[1] = 0; rec1.xy2[0] = 1; rec1.xy2[1] = 1;
		glPushMatrix();

		glTranslated(2.5,0.25,2.5);
		glScaled(5,0.5,5);
		glPushMatrix();
		glTranslated(-0.5,-0.5,0.5);
		rec1.draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.5, -0.5, -0.5);
		glRotated(180, 0, 1, 0);
		rec1.draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.5, 0.5, 0.5);
		glRotated(-90, 1, 0, 0);
		rec1.draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.5, -0.5, -0.5);
		glRotated(90, 1, 0, 0);
		rec1.draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.5, -0.5, 0.5);
		glRotated(90, 0, 1, 0);
		rec1.draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.5, -0.5, -0.5);
		glRotated(-90, 0, 1, 0);
		rec1.draw();
		glPopMatrix();
		glPopMatrix();
	}
	void draw(Texture *t){
		Rectangle rec1;
		rec1.xy1[0] = 0; rec1.xy1[1] = 0; rec1.xy2[0] = 1; rec1.xy2[1] = 1;
		glPushMatrix();

		glTranslated(5,0.5,5);
		glScaled(10,1,10);
		glPushMatrix();
		glTranslated(-0.5,-0.5,0.5);
		rec1.draw(t);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.5, -0.5, -0.5);
		glRotated(180, 0, 1, 0);
		rec1.draw(t);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.5, 0.5, 0.5);
		glRotated(-90, 1, 0, 0);
		rec1.draw(t);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.5, -0.5, -0.5);
		glRotated(90, 1, 0, 0);
		rec1.draw(t);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.5, -0.5, 0.5);
		glRotated(90, 0, 1, 0);
		rec1.draw(t);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.5, -0.5, -0.5);
		glRotated(-90, 0, 1, 0);
		rec1.draw(t);
		glPopMatrix();
		glPopMatrix();
	}

};
class Board
{
private:
	int size;
	CGFappearance *appBr;
	CGFappearance *appPr;
	CGFappearance *active;
	CGFappearance *nonActive;
	Cube cubo;


public:
	vector<vector<vector<float>>> coords;
	vector<vector<float>> plIndex;


	int activePl;
	vector<TowerP*> dsk;
	PlayerP pl1;
	PlayerP pl2;
	Board(){
		size = 0;
	}
	Board(int s,string branco,string preto){
		size=s;

		float ambient[4]; float diffuse[4]; float specular[4];
		ambient[0] = 0.5; ambient[1] = 0.5; ambient[2] = 0.5; ambient[3] = 1; 
		diffuse[0] = 0.5; diffuse[1] = 0.5; diffuse[2] = 0.5; diffuse[3] = 1;
		specular[0] = 0.5; specular[1] = 0.5; specular[2] = 0.5; specular[3] = 1;

		appBr = new CGFappearance(ambient,diffuse,specular,1);
		appPr = new CGFappearance(ambient,diffuse,specular,1);

		ambient[0] = 0.5; ambient[1] = 0.5; ambient[2] = 0.5; ambient[3] = 0.5; 
		diffuse[0] = 0.5; diffuse[1] = 0.5; diffuse[2] = 0.5; diffuse[3] = 0.5;
		specular[0] = 0.5; specular[1] = 0.5; specular[2] = 0.5; specular[3] = 0.5;

		nonActive = new CGFappearance(ambient,diffuse,specular,0.5);

		ambient[0] = 0.77; ambient[1] = 0; ambient[2] = 0; ambient[3] = 1; 
		diffuse[0] = 0.77; diffuse[1] = 0; diffuse[2] = 0; diffuse[3] = 1;
		specular[0] = 0.5; specular[1] = 0.5; specular[2] = 0.5; specular[3] =1;

		active = new CGFappearance(ambient,diffuse,specular,1);

		appBr->setTexture(branco);
		appBr->setTextureWrap(GL_REPEAT,GL_REPEAT);


		appPr->setTexture(preto);
		appPr->setTextureWrap(GL_REPEAT,GL_REPEAT);

		for(int i = 0; i < size; i++)
		{
			vector<vector<float>> line;

			for(int j = 0; j < size; j++)
			{
				vector<float> crd;
				crd.push_back(j*5.5);
				crd.push_back(1);
				crd.push_back(i*5.5);

				TowerP* Mdsk = new TowerP();

				Mdsk->pos[0] = crd[0];
				Mdsk->pos[1] = crd[1];
				Mdsk->pos[2] = crd[2];
				dsk.push_back(Mdsk);

				line.push_back(crd);
			}
			coords.push_back(line);
		}

		vector<float> pl1Index;
		pl1Index.push_back(0);
		pl1Index.push_back(size-1);

		vector<float> pl2Index;
		pl2Index.push_back(size-1);
		pl2Index.push_back(0);


		plIndex.push_back(pl1Index);
		plIndex.push_back(pl2Index);

		pl1.pos[0] = coords[plIndex[0][0]][plIndex[0][1]][0];
		pl1.pos[1] = coords[plIndex[0][0]][plIndex[0][1]][1];
		pl1.pos[2] = coords[plIndex[0][0]][plIndex[0][1]][2];


		pl2.pos[0] = coords[plIndex[1][0]][plIndex[1][1]][0];
		pl2.pos[1] = coords[plIndex[1][0]][plIndex[1][1]][1];
		pl2.pos[2] = coords[plIndex[1][0]][plIndex[1][1]][2];




	}
	void draw(){

		glPushMatrix();
		glPushName(0);
		if(activePl)
			active->apply();
		else
			nonActive->apply();
		pl1.draw();
		glPopName();
		glPopMatrix();

		glPushMatrix();
		glPushName(1);
		if(activePl)
			nonActive->apply();
		else
			active->apply();
		pl2.draw();
		glPopName();
		glPopMatrix();

		glPushMatrix();
		appBr->apply();
		for(int j=0;j<size;j++){
			for(int k=0;k<size;k++)
				dsk[k+j*size]->draw();
		}
		glPopMatrix();

		int change=0;
		glPushName(-1);	

		for(int i=0;i<size;i++){
			if(i%2==0)
				change=0;
			else 
				change=1;

			glLoadName(i);
			for(int a=0;a<size;a++){
				glPushMatrix();
				glTranslated(a*(size+0.5),0,i*(size+0.5));
				glPushName(a);
				if(a%2==change){
					//branco
					appBr->apply();
					cubo.draw();
				}
				else{
					//preto
					appPr->apply();
					cubo.draw();
				}
				glPopMatrix();
				glPopName();
			}
		}
	}
	void draw(Texture *t);
};



#endif