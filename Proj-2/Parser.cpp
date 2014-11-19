#include "Parser.h"

Parser::Parser()
{
	globals = new Globals();
	graph = new Graph();
	flag = new Flag("textures/flag.jpg");

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

void LinearAnimation::init(unsigned long t)
{
	startTime=t;
	controlTime = t;

	valid = true;
	resetTime = false;
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
	glPushMatrix();
}

void LinearAnimation::update(unsigned long t)
{
	if (doReset){
		init(t);
	}
	else
	{
		if(resetTime)
		{
			controlTime = t;
			resetTime = false;
		}
		double animT = (t-controlTime)/1000.0;
		double checkSpan = (t-startTime)/1000.0;
		if(checkSpan >= span)
		{
			valid = false;
			glPopMatrix();
		}
		else
		{
			obj_translate = velocity*animT;
		}
	}
}

void LinearAnimation::apply(){
	if(valid)
	{
		bool flag = false;
		float deltaX = directions[indexCP][0]*obj_translate,deltaY = directions[indexCP][1]*obj_translate,deltaZ = directions[indexCP][2]*obj_translate;

		float currX = initX+deltaX;
		float currY = initY+deltaY;
		float currZ = initZ+deltaZ;


		if(directions[indexCP][0] > 0)
		{
			if(currX >= controlPoint[indexCP+1][0])
				flag = true;
		}
		else if(currX <= controlPoint[indexCP+1][0])
			flag = true;

		if(flag)
		{
			if(directions[indexCP][1] > 0)
			{
				if(currY >= controlPoint[indexCP+1][1])
					flag = true;
				else
					flag = false;
			}
			else if(currY <= controlPoint[indexCP+1][1])
				flag = true;
			else
				flag = false;
		}

		if(flag)
		{
			if(directions[indexCP][2] > 0)
			{
				if(currZ >= controlPoint[indexCP+1][2])
					flag = true;
				else
					flag = false;
			}
			else if(currZ <= controlPoint[indexCP+1][2])
				flag = true;
			else
				flag = false;
		}


		if(flag)
		{
			initX = controlPoint[indexCP+1][0];
			initY = controlPoint[indexCP+1][1];
			initZ = controlPoint[indexCP+1][2];

			float product;

			if(directions[indexCP+1][1] != 0)
			{
				if(directions[indexCP+1][2] == 0)
					product = 1;
				else
					product = directions[indexCP+1][2]/(abs(directions[indexCP+1][2]));
			}
			else
				product = directions[indexCP+1][2];

			angle = acos(product)*180/PI;
			if(directions[indexCP+1][0] < 0)
				angle *= -1;
			resetTime = true;
			obj_translate = 0;
			indexCP++;
		}
		else
		{

			glTranslatef(currX,currY,currZ);
			glRotatef(angle,0,1,0);
		}




	}

}

void CircularAnimation::init(unsigned long t)
{
	startTime=t;
	valid = true;
	doReset=false;

	velocity = rotAng/span;
	radius = sqrt(center[0]*center[0]+center[1]*center[1]+center[2]*center[2]);
	obj_translate = 0;
}

void CircularAnimation::update(unsigned long t)
{
	if (doReset){
		init(t);
	}
	else
	{

		double animT = (t-startTime)/1000.0;
		if(animT >= span)
		{
			valid = false;
			glPopMatrix();
		}
		else
		{
			//obj_translate = ;
			obj_rotate = velocity*animT;
		}
	}
}

void CircularAnimation::apply(){
	if(valid)
	{
		glTranslatef(center[0],center[1],center[2]);
		glRotated(obj_rotate, 0,1,0);
		glTranslatef(-center[0],-center[1],-center[2]);
	}

}

Flag::Flag(string text){
	parts=100;
	elapsedTime=0;

	init("flag.vert", "flag.frag");

	CGFshader::bind();

	baseTexture=new CGFtexture(text);

	Timer = glGetUniformLocation(id(), "timer");

	baseImageLoc = glGetUniformLocation(id(), "baseImage");

	glUniform1i(baseImageLoc, 0);		

}

void Flag::bind(){
	CGFshader::bind();

	// update uniforms
	glUniform1f(Timer, elapsedTime);

	// make sure the correct texture unit is active
	glActiveTexture(GL_TEXTURE0);

	// apply/activate the texture you want, so that it is bound to GL_TEXTURE0
	baseTexture->apply();

}

void Flag::draw(){

	bind();
	


	float controlPoints[4][3] =
	{ { 0.5, 0, -0.5 }, { -0.5, 0, -0.5 },
	{ 0.5, 0, 0.5 }, {- 0.5, 0, 0.5 }
	};
	float controlPointsNorm[4][3] =
	{ { 0, 1, 0 }, { 0, 1, 0 },
	{ 0, 1, 0 }, { 0, 1, 0 }
	};
	float controlPointsText[4][2] =
	{ { 1, 0 }, { 0, 0 },
	{ 1, 1 }, { 0, 1 }
	};

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 6, 2, 0, 1, 3, 2, &controlPoints[0][0]);
	glMap2f(GL_MAP2_NORMAL, 0, 1, 6, 2, 0, 1, 3, 2, &controlPointsNorm[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &controlPointsText[0][0]);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glMapGrid2f(parts, 0, 1, parts, 0, 1);
	glEvalMesh2(GL_FILL, 0, parts, 0, parts);

	CGFshader::unbind();

}

void Flag::update(unsigned long t)
{
	if(elapsedTime==0){
		startTime = t;
		elapsedTime = startTime/1000.0;
	}
	else
	{
		elapsedTime = (t-startTime)/1000.0;
	}

}