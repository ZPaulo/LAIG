#include <String>
#include <vector>
#include <map>
#include <math.h>
#include "CGFapplication.h"
#include "CGFobject.h"


using namespace std;





class Drawing
{
public:
	string mode,shading;
	float background[4];
};

class Culling
{
public:
	string face, order;
};

class Lighting
{
public:
	bool doublesdd, local, enabled;
	float ambient[4];
};

class Globals
{
public:
	Drawing drawing;
	Culling culling;
	Lighting lighting;
};

class Camera
{
public:
	string id;
};

class CPerspective : public Camera
{
public:
	float near, far, angle, pos[3], target[3];
};

class COrtho : public Camera
{
public:
	char direction;
	float near,far,left,right, top, bottom;
};

class Light
{
public:
	string id,type;
	bool enabled,marker;
	float pos[3], ambient[4], diffuse[4], specular[4], target[3], angle, exponent;
};

class Texture
{
public:
	string id, file;
	float texLengthS,texLengthT;
};

class Appearance
{
public:
	string  textureRef;
	CGFappearance *appCGF;
	float ambient[4], diffuse[4], specular[4], shininness;

};

class Primitives
{
public:
	string name;
	virtual void draw() = 0;
};

class Rectangle : public Primitives
{
public:
	float xy1[2], xy2[2];
	void draw(){
		glBegin(GL_POLYGON);

		glNormal3f(xy1[0], xy1[1],1);
		glTexCoord2f(0,0);
		glVertex2f(xy1[0], xy1[1]);

		glNormal3f(xy2[0], xy1[1],1);
		glTexCoord2f(1,0);
		glVertex2f(xy2[0], xy1[1]);

		glNormal3f(xy2[0], xy2[1],1);
		glTexCoord2f(1,1);
		glVertex2f(xy2[0], xy2[1]);

		glNormal3f(xy1[0], xy2[1],1);
		glTexCoord2f(0,1);
		glVertex2f(xy1[0], xy2[1]);
		glEnd();
	}

};

class Triangle : public Primitives
{
public:
	float xyz1[3],xyz2[3], xyz3[3];
	void draw(){
		glBegin(GL_TRIANGLES);	

		glNormal3f(xyz1[0], xyz1[1],1);
		glTexCoord2f(0,0);
		glVertex3f(xyz1[0],xyz1[1],xyz1[2]);	

		glNormal3f(xyz2[0], xyz2[1],1);
		glTexCoord2f(1,0);
		glVertex3f(xyz2[0],xyz2[1],xyz2[2]);

		glNormal3f(xyz3[0], xyz3[1],1);
		glTexCoord2f(0.5,1);
		glVertex3f(xyz3[0],xyz3[1],xyz3[2]);
		glEnd();

	}
};

class Cylinder : public Primitives
{
public:
	float base, top, height;
	int slices, stacks;
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
		gluCylinder(quad,base,top,height,slices,stacks);

		glPushMatrix();
		glTranslated(0,0,height);
		gluDisk(topD,0,top,slices,stacks);
		glPopMatrix();

		glPushMatrix();
		glRotated(180,0,1,0);
		gluDisk(botD,0,base,slices,stacks);
		glPopMatrix();

		gluDeleteQuadric(quad);
		gluDeleteQuadric(botD);
		gluDeleteQuadric(topD);

	}
};

class Sphere : public Primitives
{
public:
	float radius;
	int slices, stacks;
	void draw(){

		GLUquadric *sphere = gluNewQuadric();
		gluQuadricTexture(sphere, GL_TRUE);
		gluQuadricNormals(sphere,GL_SMOOTH);

		gluSphere(sphere,radius,slices,stacks);

		gluDeleteQuadric(sphere);

	}
};

class Torus : public Primitives
{
public:

	float inner, outer;
	int slices, loops;
	void draw(){
		float pi = acos(-1.0);
		float deg2rad=pi/180.0;
		double TWOPI = 2 * pi;
		for (int i = 0; i < slices; i++) {
			glBegin(GL_QUAD_STRIP);
			for (int j = 0; j <= loops; j++) {
				for (int k = 1; k >= 0; k--) {

					double s = (i + k) % slices + 0.5;
					double t = j % loops;

					double x = (inner + 0.1* cos(s * TWOPI / slices)) * cos(t * TWOPI / loops);
					double y = (inner + 0.1* cos(s * TWOPI / slices)) * sin(t * TWOPI / loops);
					double z = 0.1* sin(s * TWOPI / slices);

					glVertex3d(2 * x, 2 * y, 2 * z);
				}
			}
			glEnd();
		}

	}
};

class Node
{
public:
	string id;
	float matrix[16];
	string apperanceRef;
	vector<Primitives*> primitives;
	vector<string> descendants;
};

class Graph
{
public:
	map<string, Node*> nodes;
	string rootID;
};

class Parser
{
public:
	Parser();
	~Parser();
	Globals* globals;
	map <string, Camera> cameras;
	string initCam;
	string activeCam;
	vector<Light*> lights;
	map<string, Texture*> textures;
	map<string, Appearance*> appearances;
	Graph* graph;
};