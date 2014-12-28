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
private:
	float base, top, height;
	int slices, stacks;
public:
	TowerP(){
		base=4; top=4; height=1;
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
		glTranslated(5,1,5);
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
		glTranslated(5,1,5);
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
class PlayerP
{
public:
	PlayerP();
	void draw(){
		
	}
	void draw(Texture *t){

	}
};
class Board
{
private:
	int size;
	Texture *texBranco;
	Texture *texPreto;

public:
	Board(int s,string branco,string preto){
		size=s;
		Texture *texBranco = new Texture();
		Texture *texPreto = new Texture();
		texBranco->file = branco;
		texPreto->file = preto;
		texBranco->texLengthS = 1;
		texBranco->texLengthT = 1;
		texPreto->texLengthS = 1;
		texPreto->texLengthT = 1;
		texBranco->id="branco";
		texPreto->id="preto";
		///////////////////////////////////////definir texturas pelas strings
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
					cubo.draw();/////////////////////////////////////////////////////textura branca	
				}
				else{
					//preto
					cubo.draw();////////////////////////////////////////////////////////textura preto
					TowerP o;
					o.draw();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
	}
	void draw(Texture *t);
};
#endif