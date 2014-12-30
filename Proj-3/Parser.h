#ifndef _PARSER_H_
#define _PARSER_H_


#include <String>
#include <vector>
#include <map>
#include <math.h>
#include <CGFapplication.h>

using namespace std;

#define PI  3.14159265359


class Drawing
{
public:
	string shading;
	int mode;
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

class Camera : public CGFcamera
{
public:
	string id;
	virtual void apply() = 0;
};

class CPerspective : public Camera
{
public:
	float near, far, angle, pos[3], target[3];
	void apply();
};

class COrtho : public Camera
{
public:
	string direction;
	float near,far,left,right, top, bottom;
	void apply();
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

class Animation
{
public:
	string id,type;
	float span;

	virtual void apply() = 0;
	virtual void update(unsigned long t) = 0;
	bool valid,doReset,isSequence;

private:
	virtual void init(unsigned long t) = 0;
	unsigned long startTime;

};

class LinearAnimation : public Animation
{
public:
	vector<vector<float>> controlPoint;
	void apply();
	void update(unsigned long t);


private:
	void init(unsigned long t);
	unsigned long startTime, controlTime;
	unsigned int indexCP;
	vector<vector<float>> directions;
	float initX,initY,initZ, angle;
	bool resetTime;
	float  obj_translate,velocity;
};

class CircularAnimation : public Animation
{
public:
	float span,radius,startAng,rotAng,center[3];

	void apply();
	void update(unsigned long t);

private:
	void init(unsigned long t);
	float  obj_translate,velocity,obj_rotate;
	unsigned long startTime;
	vector<vector<float>> directions;
};

class Appearance
{
public:
	string  textureRef;
	CGFappearance *appCGF;
	float ambient[4], diffuse[4], specular[4], shininness;
	bool isTexApp;

};

class Primitives
{
public:
	string name;
	virtual void draw() = 0;
	virtual void draw(Texture *t) = 0;
};

class Rectangle : public Primitives
{
public:
	float xy1[2], xy2[2];
	void draw(Texture *t);
	void draw();
};

class Triangle : public Primitives
{
public:
	float xyz1[3],xyz2[3], xyz3[3];
	void draw();
	void draw(Texture *t);
};

class Cylinder : public Primitives
{
public:
	float base, top, height;
	int slices, stacks;
	void draw();
	void draw(Texture *t);
};

class Sphere : public Primitives
{
public:
	float radius;
	int slices, stacks;
	void draw();
	void draw(Texture *t);
};

class Torus : public Primitives
{
public:

	float inner, outer;
	int slices, loops;
	void draw();
	void draw(Texture *t);
};

class Plane:public Primitives{
public:
	int parts;
	void draw();
	void draw(Texture *t);
};

class Patch: public Primitives{
public:
	int order;
	int partsU;
	int partsV;
	string compute;
	vector<vector<GLfloat>> controlPoint;
	void draw();
	void draw(Texture *t);
};

class Vehicle : public Primitives{
public:
	float angleF, angleB;
	
	float ambC[4], difC[4], specC[4], shininessC;
	float ambF[4], difF[4], specF[4], shininessF;
	float ambV[4], difV[4], specV[4], shininessV;
	CGFappearance* metal;
	CGFappearance* vidro;
	CGFappearance* metal2;
	CGFappearance* default1;

	Vehicle();
	void pes(Cylinder cil);
	void connector(Cylinder cil);
	Patch front(Patch back);
	Patch base1(Patch back);
	Patch base2(Patch back);
	Patch backD(Patch back);
	Patch roof(Patch back);
	void cube();
	void setangleF();
	void setangleB();
	void draw();
	void draw(Texture *t);
};

class Node
{
public:
	string id;
	float matrix[16];
	string apperanceRef;
	vector<Animation*> animation;
	vector<Primitives*> primitives;
	unsigned int animIndex;
	bool displayList;
	int indexDList;
	vector<string> descendants;

	~Node();
};

class Graph
{
public:
	map<string, Node*> nodes;
	string rootID;
	~Graph();
};

class Parser
{
public:
	Parser();
	~Parser();
	Globals* globals;
	vector<Camera*> cameras;
	string initCam;
	int activeCam;
	vector<Light*> lights;
	map<string, Texture*> textures;
	map<string, Appearance*> appearances;
	vector<Animation*> animations;
	int wind;


	Graph* graph;
};

#endif