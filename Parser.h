#include <String>
#include <vector>

using namespace std;

class Parser
{
public:
	Parser(void);
	~Parser(void);
};

class Globals
{
public:
	Drawing drawing;
	Culling culling;
	Lighting lighting;
};

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

class Cameras
{
public:
	string initCam;
	vector<CPerspective> pCamera;
	vector<COrtho> oCamera;
	string activeCam;

};

class CPerspective
{
public:
	string id;
	float near, far, angle, pos[3], target[3];
};

class COrtho
{
public:
	string id;
	char direction;
	float far,left,right, top, bottom;
};