#ifndef _PIECES
#define _PIECES

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <String>
#include <vector>
#include <map>
#include <math.h>
#include <CGFapplication.h>
#include <CGFshader.h>
#include <CGFtexture.h>

class Texture
{
public:
	string id, file;
	float texLengthS,texLengthT;
};
class Sphere
{
private:
	float radius;
	int slices, stacks;
public:
	Sphere(){
		radius=1.5;
		slices=20;
		stacks=20;
	}
	
	void draw(){
		glPushMatrix();
		glTranslated(5,6,5);
		GLUquadric *sphere = gluNewQuadric();
		gluQuadricTexture(sphere, GL_TRUE);
		gluQuadricNormals(sphere,GL_SMOOTH);

		gluSphere(sphere,radius,slices,stacks);

		gluDeleteQuadric(sphere);
		glPopMatrix();
	}
	void draw(Texture *t){
		glPushMatrix();
		glTranslated(5,6,5);
		GLUquadric *sphere = gluNewQuadric();
		gluQuadricTexture(sphere, GL_TRUE);
		gluQuadricNormals(sphere,GL_SMOOTH);

		gluSphere(sphere,radius,slices,stacks);

		gluDeleteQuadric(sphere);
		glPopMatrix();
	}

};
class Cylinder
{
private:
	float base, top, height;
	int slices, stacks;
public:
	Cylinder(float b, float t){
		base=b; top=t; height=1;
		slices=20; stacks=20;
	}
	void draw(){
		GLUquadric * quad,*botD,*topD;

		quad = gluNewQuadric();
		botD = gluNewQuadric();
		topD = gluNewQuadric();


		gluQuadricNormals(quad,GL_SMOOTH);
		gluQuadricNormals(botD,GL_SMOOTH);
		gluQuadricNormals(topD,GL_SMOOTH);


		gluQuadricTexture(quad, GL_TRUE);
		gluQuadricTexture(botD, GL_TRUE);
		gluQuadricTexture(topD, GL_TRUE);
		glPushMatrix();
		glTranslated(5,0.5,5);
		glRotated(-90,1,0,0);
		gluCylinder(quad,base,top,height,slices,stacks);

		glPushMatrix();
		glTranslated(0,0,height);
		gluDisk(topD,0,top,slices,stacks);
		glPopMatrix();

		glPushMatrix();
		glRotated(180,0,1,0);
		gluDisk(botD,0,base,slices,stacks);
		glPopMatrix();
		glPopMatrix();
		gluDeleteQuadric(quad);
		gluDeleteQuadric(botD);
		gluDeleteQuadric(topD);
	}
	void draw(Texture *t){
		GLUquadric * quad,*botD,*topD;

		quad = gluNewQuadric();
		botD = gluNewQuadric();
		topD = gluNewQuadric();


		gluQuadricNormals(quad,GL_SMOOTH);
		gluQuadricNormals(botD,GL_SMOOTH);
		gluQuadricNormals(topD,GL_SMOOTH);


		gluQuadricTexture(quad, GL_TRUE);
		gluQuadricTexture(botD, GL_TRUE);
		gluQuadricTexture(topD, GL_TRUE);
		glPushMatrix();
		glTranslated(5,0.5,5);
		glRotated(-90,1,0,0);
		gluCylinder(quad,base,top,height,slices,stacks);

		glPushMatrix();
		glTranslated(0,0,height);
		gluDisk(topD,0,top,slices,stacks);
		glPopMatrix();

		glPushMatrix();
		glRotated(180,0,1,0);
		gluDisk(botD,0,base,slices,stacks);
		glPopMatrix();
		glPopMatrix();
		gluDeleteQuadric(quad);
		gluDeleteQuadric(botD);
		gluDeleteQuadric(topD);
	}

};
class Rectangle
{
public:
	float xy1[2], xy2[2];
	void draw(Texture *t){
	float lenT = t->texLengthS, heiT = t->texLengthT, lenO = xy2[0]-xy1[0],heiO = xy2[1]-xy1[1];
	float xT,yT;
	xT = lenO/lenT;
	yT = heiO/heiT;
	glBegin(GL_POLYGON);

	glNormal3f(0,0,1);
	glTexCoord2f(0,0); 
	glVertex2f(xy1[0], xy1[1]);

	glTexCoord2f(xT,0);
	glVertex2f(xy2[0], xy1[1]);

	glTexCoord2f(xT,yT);
	glVertex2f(xy2[0], xy2[1]);

	glTexCoord2f(0,yT);
	glVertex2f(xy1[0], xy2[1]);

	glEnd();
	}
	void draw(){
		glBegin(GL_POLYGON);

	glNormal3f(0,0,1);
	glTexCoord2f(0,0); 
	glVertex2f(xy1[0], xy1[1]);
	
	glTexCoord2f(1,0);
	glVertex2f(xy2[0], xy1[1]);

	glTexCoord2f(1,1);
	glVertex2f(xy2[0], xy2[1]);

	glTexCoord2f(0,1);
	glVertex2f(xy1[0], xy2[1]);

	glEnd();
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
		glScaled(10,1,10);
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
class TowerP
{
public:
	void draw(){
		Cylinder cil(4,4);
		cil.draw();
	}
	void draw(Texture *t){
		Cylinder cil(4,4);
		cil.draw(t);
	}
};
class PlayerP
{
public:
	void draw(){
		glPushMatrix();
		glTranslated(0,-1.,0);
		glScaled(1,5,1);
		Cylinder cil(4,0);
		cil.draw();
		glPopMatrix();

		glPushMatrix();
		Sphere sp;
		sp.draw();
		glPopMatrix();
	}
	void draw(Texture *t){
		glPushMatrix();
		glTranslated(0,-1.,0);
		glScaled(1,5,1);
		Cylinder cil(4,0);
		cil.draw(t);
		glPopMatrix();

		glPushMatrix();
		Sphere sp;
		sp.draw(t);
		glPopMatrix();
	}
};
class Board
{
private:
	int size;
	CGFappearance *appBr;
	CGFappearance *appPr;

public:
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
		Cube cubo;
		int change=0;
		for(unsigned int i=0;i<size;i++){
			if(i%2==0)
				change=0;
			else 
				change=1;
			glPushMatrix();
			glTranslated(0,0,i*10);
			for(unsigned int a=0;a<size;a++){
				glPushMatrix();
				glTranslated(a*10,0,0);
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
				glPopMatrix();
			}
			glPopMatrix();
		}
	}
	void draw(Texture *t);
};
#endif