#include "TextObject.h"
#include <GL/glut.h>

void TextObject::draw(int player)
{

	// desativa cálculo de iluminação para usar cores diretamente
	glDisable(GL_LIGHTING);


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

	/*switch (num)
	{
	case 0: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0');
		break;
	case 1: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '1');
		break;
	case 2: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '2');
		break;
	case 3: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '3');
		break;
	case 4: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '4');
		break;
	case 5: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '5');
		break;
	case 6: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '6');
		break;
	case 7: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '7');
		break;
	case 8: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '8');
		break;
	case 9: 
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '9');
		break;
	default:
		break;
	}*/



	glPushMatrix();
	glTranslatef(0,-3,0);
	glColor3f(1.0,1.0,0.0);		// amarelo
	glRasterPos3f(5,5,5);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'P');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '/');
	if(player)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '2');
	else
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '1');
	glPopMatrix();

	glEnable(GL_LIGHTING);

}