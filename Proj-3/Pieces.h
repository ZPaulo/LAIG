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
	TowerP(){
		diskAppearance = new CGFappearance();
		cil.base = 2.5;
		cil.top = 2.5;
		cil.height = 0.5;
		cil.slices=20; 
		cil.stacks=20;
	}
	void draw(){
		cil.draw();
	}
	void draw(Texture *t){
		cil.draw(t);
	}
};
class PlayerP
{
public:
	Cylinder cil;
	Sphere sp;
	PlayerP(){
		cil.base = 1.5;
		cil.top = 0;
		cil.height = 1;
		cil.slices=20; 
		cil.stacks=20;

		sp.radius=1.5;
		sp.slices=20;
		sp.stacks=20;
	}
	void draw(){
		glPushMatrix();
		glTranslated(2.5,0,2.5);

		glPushMatrix();
		glScaled(1,5,1);
		glRotatef(-90,1,0,0);
		glTranslated(0,-1.,0);
		cil.draw();
		glPopMatrix();


		//sp.draw();
		glPopMatrix();
	}
	void draw(Texture *t){
		glPushMatrix();
		glTranslated(0,-1.,0);
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

		glTranslated(2.5,0,2.5);
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

		glTranslated(5,0,5);
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
	Cube cubo;
	

public:
	vector<vector<vector<float>>> coords;
	TowerP dsk;
	PlayerP pl1;
	PlayerP pl2;
	Board(){
		size = 0;
	}
	Board(int s,string branco,string preto){
		size=s;

		float ambient[4]; float diffuse[4]; float specular[4];
		ambient[0] = 0.5; ambient[1] = 0.5; ambient[2] = 0.5; ambient[3] = 0.5; 
		diffuse[0] = 0.5; diffuse[1] = 0.5; diffuse[2] = 0.5; diffuse[3] = 0.5;
		specular[0] = 0.5; specular[1] = 0.5; specular[2] = 0.5; specular[3] = 0.5;

		appBr = new CGFappearance(ambient,diffuse,specular,1);
		appPr = new CGFappearance(ambient,diffuse,specular,1);

		appBr->setTexture(branco);
		appBr->setTextureWrap(GL_REPEAT,GL_REPEAT);


		appPr->setTexture(preto);
		appPr->setTextureWrap(GL_REPEAT,GL_REPEAT);
		
	}
	void draw(){
		int change=0;
		glPushMatrix();
		glTranslatef(0,3,(size-1)*5.5);
		glPushName(1);
		pl1.draw();
		glPopName();
		glPopMatrix();
		glPushMatrix();
		glTranslatef((size-1)*5.5,3,0);
		glPushName(2);
		pl2.draw();
		glPopName();
		glPopMatrix();
		glPushName(-1);		// Load a default name

		for(int i=0;i<size;i++){
			if(i%2==0)
				change=0;
			else 
				change=1;
			glPushMatrix();
			glTranslated(0,0,i*5.5);
			glLoadName(i);
			vector<vector<float>> line;
			for(int a=0;a<size;a++){
				glPushMatrix();
				glTranslated(a*5.5,0,0);
				vector<float> crd;
				crd.push_back(a*5.5);
				crd.push_back(0);
				crd.push_back(i*5.5);
				line.push_back(crd);
				
				glPushName(a);
				
					if(a%2==change){
					//branco
					appBr->apply();
					cubo.draw();
				}
				else{
					//preto
					//appPr->apply();
					cubo.draw();
				}
				
				glPushMatrix();
				glTranslatef(2.5,0.7,2.5);
				glRotatef(90,1,0,0);
				dsk.diskAppearance->apply();
				dsk.draw();
				glPopMatrix();
				glPopName();
				glPopMatrix();
			}
			coords.push_back(line);
			glPopMatrix();
		}
	}
	void draw(Texture *t);
};



#endif