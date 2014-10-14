#include <String>
#include <vector>
#include <map>
#include "ANFScene.h"
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
	string id, textureRef;
	float ambient[4], diffuse[4], specular[4];

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
		glVertex2f(xy1[0], xy1[1]);
		glVertex2f(xy2[0], xy1[1]);
		glVertex2f(xy2[0], xy2[1]);
		glVertex2f(xy1[0], xy2[1]);
		glEnd();
	}

};

class Triangle : public Primitives
{
public:
	float xyz1[3],xyz2[3], xyz3[3];
	vector<float> getCoord(){
		vector<float> coord;
		coord.push_back(xyz1[0]);
		coord.push_back(xyz1[1]);
		coord.push_back(xyz1[2]);
		coord.push_back(xyz2[0]);
		coord.push_back(xyz2[1]);
		coord.push_back(xyz2[2]);
		coord.push_back(xyz3[0]);
		coord.push_back(xyz3[1]);
		coord.push_back(xyz3[2]);

		return coord;
	}
};

class Cylinder : public Primitives
{
public:
	float base, top, height;
	int slices, stacks;
	vector<float> getCoord(){
		vector<float> coord;
		coord.push_back(base);
		coord.push_back(top);
		coord.push_back(height);
		coord.push_back(slices);
		coord.push_back(stacks);

		return coord;
	}
};

class Sphere : public Primitives
{
public:
	float radius;
	int slices, stacks;
	vector<float> getCoord(){
		vector<float> coord;
		coord.push_back(radius);
		coord.push_back(slices);
		coord.push_back(stacks);

		return coord;
	}
};

class Torus : public Primitives
{
public:
	float inner, outer;
	int slices, loops;
	vector<float> getCoord(){
		vector<float> coord;
		coord.push_back(inner);
		coord.push_back(outer);
		coord.push_back(loops);
		coord.push_back(slices);

		return coord;
	}
};

class Node
{
public:
	string id;
	float matrix[16];
	Appearance* apperance;
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
	map<string, Texture> textures;
	map<string, Appearance> appearances;
	Graph* graph;
};