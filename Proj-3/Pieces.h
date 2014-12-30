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
	TowerP(){
		cil.base = 4;
		cil.top = 4;
		cil.height = 1;
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
		cil.base = 4;
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
		glTranslated(0,-1.,0);
		glScaled(1,5,1);
		cil.draw();
		glPopMatrix();


		sp.draw();
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

		glTranslated(5,0,5);
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
	Board(){
		size = 0;
	}
	Board(int s,string branco,string preto){
		size=s;

		///////////////////////////////////////definir texturas pelas 
		/*

		float ambient[4]; float diffuse[4]; float specular[4];
		ambient[0] = 0.5; ambient[1] = 0.5; ambient[2] = 0.5; ambient[3] = 0.5; 
		diffuse[0] = 0.5; diffuse[1] = 0.5; diffuse[2] = 0.5; diffuse[3] = 0.5;
		specular[0] = 0.5; specular[1] = 0.5; specular[2] = 0.5; specular[3] = 0.5;

		CGFappearance *appBr = new CGFappearance(ambient,diffuse,specular,1);
		CGFappearance *appPr = new CGFappearance(ambient,diffuse,specular,1);

		if(FILE *file = fopen(branco.c_str(), "r")) 
		{
		printf("ooooooooooooo\n");
		}
		appBr->setTexture(branco);
		appBr->setTextureWrap(GL_REPEAT,GL_REPEAT);


		appPr->setTexture(preto);
		appPr->setTextureWrap(GL_REPEAT,GL_REPEAT);
		*/
	}
	void draw(){
		int change=0;

		glPushName(-1);		// Load a default name

		for(int i=0;i<size;i++){
			if(i%2==0)
				change=0;
			else 
				change=1;
			glPushMatrix();
			glTranslated(0,0,i*5);
			glLoadName(i);
			for(int a=0;a<size;a++){
				glPushMatrix();
				glTranslated(a*5,0,0);
				glPushName(a);
				if(a%2==change){
					//branco
					////////////appBr->apply();
					cubo.draw();/////////////////////////////////////////////////////textura branca	
				}
				else{
					//preto
					/////////////->apply();
					cubo.draw();////////////////////////////////////////////////////////textura preto
				}
				glPopName();
				glPopMatrix();
			}
			glPopMatrix();
		}
	}
	void draw(Texture *t);
};

class Pieces
{
public:
};


#endif