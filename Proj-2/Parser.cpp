#include "Parser.h"


Parser::Parser()
{
	globals = new Globals();
	graph = new Graph();

}


Parser::~Parser()
{
	while(!cameras.empty()) 
	{
		delete cameras.back(); cameras.pop_back();
	}
	while(!lights.empty()) 
	{
		delete lights.back(); lights.pop_back();
	}
	textures.clear();
	appearances.clear();
	delete(globals);
	delete(graph);
}

Node::~Node()
{
	descendants.clear();
	primitives.clear();

}

Graph::~Graph()
{
	nodes.clear();
}

void Animation::init(unsigned long t)
{
	startTime=t;
	valid = true;
	indexCP = 0;
	doReset=false;

	float distance = 0;

	for(unsigned int i = 0; i < controlPoint.size()-1;i++)
	{
		vector<float> dir;
		float x,y,z, norm;
		x = controlPoint[i+1][0]-controlPoint[i][0];
		y = controlPoint[i+1][1]-controlPoint[i][1];
		z = controlPoint[i+1][2]-controlPoint[i][2];

		norm = sqrt(x*x+y*y+z*z);
		dir.push_back(x/norm);
		dir.push_back(y/norm);
		dir.push_back(z/norm);

		distance += norm;

		directions.push_back(dir);
	}

	float product = directions[0][2];
	angle = acos(product)*180/PI;
	

	velocity = distance / span;

	initX = controlPoint[0][0];
	initY = controlPoint[0][1];
	initZ = controlPoint[0][2];

	obj_translate = 0;
}

void Animation::update(unsigned long t)
{
	if (doReset)
		init(t);
	else
	{
		double animT = (t-startTime)/1000.0;
		if(animT >= span)
			valid = false;
		else
		{
			//obj_rotate= START_ANGLE + animT* rotate_speed_ms;
			//obj_radius= START_RADIUS + animT* radius_speed_ms;
			obj_translate = velocity*animT;
		}
	}
}

void Animation::apply(){
	if(valid)
	{
		float deltaX = directions[indexCP][0]*obj_translate,deltaY = directions[indexCP][1]*obj_translate,deltaZ = directions[indexCP][2]*obj_translate;

		float currX = initX+deltaX;
		float currY = initY+deltaY;
		float currZ = initZ+deltaZ;

		glTranslatef(currX,currY,currZ);
		glRotatef(angle,0,1,0);



		if(indexCP < directions.size() -1)
			if(((int) currX == controlPoint[indexCP+1][0]) && ((int)currY == controlPoint[indexCP+1][1]) && ((int)currZ == controlPoint[indexCP+1][2]))
			{
				initX = controlPoint[indexCP+1][0]*2;
				initY = controlPoint[indexCP+1][1]*2;
				initZ = controlPoint[indexCP+1][2]*2;
				float product = directions[indexCP][0]*directions[indexCP+1][0]+directions[indexCP][2]*directions[indexCP+1][2];
				angle = acos(product)*180/PI;
				indexCP++;
			}

	}

}