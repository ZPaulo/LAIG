#include "TextObject.h"
#include <GL/glut.h>

void TextObject::draw(int player)
{
	// Texto 3D, GLUT
	// void glutStrokeCharacter(void *font, int character);	// GLUT_STROKE_ROMAN
	// int glutStrokeWidth(GLUTstrokeFont font, int character);

	// desativa cálculo de iluminação para usar cores diretamente
	glDisable(GL_LIGHTING);

	/*glPushMatrix();
	glTranslatef(0,0,1);
	glScalef(0.05, 0.05, 0.05);
	glColor3f(0.0,0.0,1.0);		// azul
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'P');
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'L');
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'A');
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'Y');
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'E');
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'R');
	glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
	if(player)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, '2');
	else
		glutStrokeCharacter(GLUT_STROKE_ROMAN, '1');
	glPopMatrix();*/

	// Texto BitMap, GLUT
	// glRasterPos3f(x,y,z);
	// void glutBitmapCharacter(void *font, int character);	// valores varios...
	// int glutBitmapWidth(GLUTbitmapFont font, int character);

	glPushMatrix();
	glColor3f(1.0,1.0,0.0);		// amarelo
	glRasterPos3f(5,5,5);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'P');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Y');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	if(player)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '2');
	else
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '1');
	glPopMatrix();

	glEnable(GL_LIGHTING);

}