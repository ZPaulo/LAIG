#include "Parser.h"


Parser::Parser()
{
	globals = new Globals();
	graph = new Graph();
	wind = 0;


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
	isSequence = false;
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
			indexCP++;
			initX = controlPoint[indexCP][0];
			initY = controlPoint[indexCP][1];
			initZ = controlPoint[indexCP][2];

			float product;

			if(directions[indexCP][1] != 0)
			{
				if(directions[indexCP][2] == 0)
					product = 1;
				else
					product = directions[indexCP][2]/(abs(directions[indexCP][2]));
			}
			else
				product = directions[indexCP][2];

			angle = acos(product)*180/PI;
			if(directions[indexCP][0] < 0)
				angle *= -1;
			resetTime = true;
			obj_translate = 0;

		}
		else
		{

			glTranslatef(currX,currY,currZ);
			glRotatef(angle,0,1,0);
		}
	}
	else if(isSequence){
		glTranslatef(controlPoint[controlPoint.size()-1][0],controlPoint[controlPoint.size()-1][1],controlPoint[controlPoint.size()-1][2]);
		glRotatef(angle,0,1,0);
	}

}

void CircularAnimation::init(unsigned long t)
{
	startTime=t;
	valid = true;
	isSequence = false;
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
			obj_rotate = velocity*animT;
		}
	}
}

void CircularAnimation::apply(){
	if(valid)
	{
		glTranslatef(center[0],center[1],center[2]);
		glRotatef(obj_rotate, 0,1,0);
		glTranslatef(-center[0],-center[1],-center[2]);

	}
	else if(isSequence){
		glTranslatef(center[0],center[1],center[2]);
		glRotated(obj_rotate, 0,1,0);
		glTranslatef(-center[0],-center[1],-center[2]);
	}

}

void CPerspective::apply(){

	float ratio = ((float) CGFapplication::width)/((float) CGFapplication::height);

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle, ratio, near1, far1 );
	glPopMatrix();

	gluLookAt(pos[0],pos[1],pos[2],target[0],target[1],target[2],0,1,0);
}

void COrtho::apply(){
	float ratio = ((float) CGFapplication::width)/((float) CGFapplication::height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(left * ratio,right*ratio ,bottom,top, near1, far1);
	if(direction == "x")
		glRotatef(-90,0,1,0);
	if(direction == "y")
		glRotatef(90,1,0,0);
}

void Rectangle::draw(Texture *t){
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
void Rectangle::draw(){

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

void Triangle::draw(){
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
void Triangle::draw(Texture *t){
	float lenT = t->texLengthS, heiT = t->texLengthT, lenO = xyz1[0]-xyz2[0],heiO = xyz3[1]-xyz1[1];
	float xT,yT;

	xT = lenO/lenT;
	yT = heiO/heiT;

	glBegin(GL_TRIANGLES);	

	glNormal3f(xyz1[0], xyz1[1],1);
	glTexCoord2f(0,0);
	glVertex3f(xyz1[0],xyz1[1],xyz1[2]);	

	glNormal3f(xyz2[0], xyz2[1],1);
	glTexCoord2f(xT,0);
	glVertex3f(xyz2[0],xyz2[1],xyz2[2]);

	glNormal3f(xyz3[0], xyz3[1],1);
	glTexCoord2f(xT*0.5,yT);
	glVertex3f(xyz3[0],xyz3[1],xyz3[2]);
	glEnd();

}

void Cylinder::draw(){

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
void Cylinder::draw(Texture *t){

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

void Sphere::draw(){

	GLUquadric *sphere = gluNewQuadric();
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere,GL_SMOOTH);

	gluSphere(sphere,radius,slices,stacks);

	gluDeleteQuadric(sphere);

}
void Sphere::draw(Texture *t){

	GLUquadric *sphere = gluNewQuadric();
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere,GL_SMOOTH);

	gluSphere(sphere,radius,slices,stacks);

	gluDeleteQuadric(sphere);

}

void Torus::draw(){
	int i, j;
	GLfloat theta, phi, theta1;
	GLfloat cosTheta, sinTheta;
	GLfloat cosTheta1, sinTheta1;
	GLfloat ringDelta, sideDelta;

	ringDelta = 2.0 * PI / loops;
	sideDelta = 2.0 * PI / slices;

	theta = 0.0;
	cosTheta = 1.0;
	sinTheta = 0.0;
	for (i = loops - 1; i >= 0; i--) {
		theta1 = theta + ringDelta;
		cosTheta1 = cos(theta1);
		sinTheta1 = sin(theta1);
		glBegin(GL_QUAD_STRIP);
		phi = 0.0;
		for (j = slices; j >= 0; j--) {
			GLfloat cosPhi, sinPhi, dist;

			phi += sideDelta;
			cosPhi = cos(phi);
			sinPhi = sin(phi);
			dist = outer + inner * cosPhi;

			glNormal3f(cosTheta1 * cosPhi, -sinTheta1  *cosPhi, sinPhi);
			//glTexCoord3f(cosTheta1 * cosPhi, -sinTheta1  *cosPhi, sinPhi);
			glVertex3f(cosTheta1  *dist, -sinTheta1  *dist, inner * sinPhi);
			glNormal3f(cosTheta  *cosPhi, -sinTheta  *cosPhi, sinPhi);
			//glTexCoord3f(cosTheta  *cosPhi, -sinTheta  *cosPhi, sinPhi);
			glVertex3f(cosTheta  *dist, -sinTheta * dist,  inner * sinPhi);
		}
		glEnd();
		theta = theta1;
		cosTheta = cosTheta1;
		sinTheta = sinTheta1;
	}
}
void Torus::draw(Texture *t){
	float lenT = t->texLengthS, heiT = t->texLengthT;
	int i, j;
	GLfloat theta, phi, theta1;
	GLfloat cosTheta, sinTheta;
	GLfloat cosTheta1, sinTheta1;
	GLfloat ringDelta, sideDelta;

	ringDelta = 2.0 * PI / loops;
	sideDelta = 2.0 * PI / slices;

	theta = 0.0;
	cosTheta = 1.0;
	sinTheta = 0.0;
	for (i = loops - 1; i >= 0; i--) {
		theta1 = theta + ringDelta;
		cosTheta1 = cos(theta1);
		sinTheta1 = sin(theta1);
		glBegin(GL_QUAD_STRIP);
		phi = 0.0;
		for (j = slices; j >= 0; j--) {
			GLfloat cosPhi, sinPhi, dist;

			phi += sideDelta;
			cosPhi = cos(phi);
			sinPhi = sin(phi);
			dist = outer + inner * cosPhi;

			glNormal3f(cosTheta1 * cosPhi, -sinTheta1  *cosPhi, sinPhi);
			//glTexCoord2f((j) / slices, ((j) / slices-i) / loops);
			if(cosTheta1 * (cosPhi)>0)
				glTexCoord3f(cosTheta1 * (cosPhi)*lenT, -sinTheta1  *cosPhi*heiT, -sinPhi);
			else
				glTexCoord3f(cosTheta1 * (-cosPhi)*lenT, -sinTheta1  *cosPhi*heiT, -sinPhi);
			glVertex3f(cosTheta1  *dist, -sinTheta1  *dist, inner * sinPhi);

			glNormal3f(cosTheta  *cosPhi, -sinTheta  *cosPhi, sinPhi);
			//glTexCoord2f((j-1) / slices, ((j-1) / slices-i) / loops);
			if(cosTheta * (cosPhi)>0)
				glTexCoord3f(cosTheta  *(cosPhi)*lenT, -sinTheta  *cosPhi*heiT, -sinPhi);
			else
				glTexCoord3f(cosTheta  *(-cosPhi)*lenT, -sinTheta  *cosPhi*heiT, -sinPhi);
			glVertex3f(cosTheta  *dist, -sinTheta * dist,  inner * sinPhi);


		}
		glEnd();
		theta = theta1;
		cosTheta = cosTheta1;
		sinTheta = sinTheta1;
	}
}

void Plane::draw(){
	float controlPoints[4][3] =
	{ { 0.5, 0, -0.5 }, { -0.5, 0, -0.5 },
	{ 0.5, 0, 0.5 }, { -0.5, 0, 0.5 }
	};
	float controlPointsNorm[4][3] =
	{ { 0, 1, 0 }, { 0, 1, 0 },
	{ 0, 1, 0 }, { 0, 1, 0 }
	};

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 6, 2, 0, 1, 3, 2, &controlPoints[0][0]);
	glMap2f(GL_MAP2_NORMAL, 0, 1, 6, 2, 0, 1, 3, 2, &controlPointsNorm[0][0]);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glMapGrid2f(parts, 0, 1, parts, 0, 1);
}
void Plane::draw(Texture *t){
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
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 4, 2, 0, 1, 2, 2, &controlPointsText[0][0]);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glMapGrid2f(parts, 0, 1, parts, 0, 1);
	glEvalMesh2(GL_FILL, 0, parts, 0, parts);

}

void Patch::draw(){
	glFrontFace(GL_CW);
	float controlPoint2[100][3];
	for (unsigned int i = 0; i < controlPoint.size(); i++)
		for (int f = 0; f < 3; f++)
			controlPoint2[i][f] = controlPoint[i][f];

	int x = controlPoint.size();

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, (order + 1), 0, 1, (order + 1) * 3, x / (order + 1), &controlPoint2[0][0]);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);

	glMapGrid2f(partsU, 0, 1, partsV, 0, 1);
	if (compute == "point")
		glEvalMesh2(GL_POINT, 0, partsU, 0, partsV);
	else if (compute == "line")
		glEvalMesh2(GL_LINE, 0, partsU, 0, partsV);
	else
		glEvalMesh2(GL_FILL, 0, partsU, 0, partsV);

	glFrontFace(GL_CCW);
}
void Patch::draw(Texture *t){
	glFrontFace(GL_CW);
	float controlPointsText[4][2] =
	{ { 1, 0 }, { 0, 0 },
	{ 1, 1 }, { 0, 1 }
	};

	float controlPoint2[16][3];
	for (unsigned int i = 0; i < controlPoint.size(); i++)
		for (int f = 0; f < 3; f++)
			controlPoint2[i][f] = controlPoint[i][f];


	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, (order + 1), 0, 1, (order + 1) * 3, order + 1, &controlPoint2[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 4, 2, 0, 1, 2, 2, &controlPointsText[0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMapGrid2f(partsU, 0, 1, partsV, 0, 1);
	if (compute == "point")
		glEvalMesh2(GL_POINT, 0, partsU, 0, partsV);
	else if (compute == "line")
		glEvalMesh2(GL_LINE, 0, partsU, 0, partsV);
	else
		glEvalMesh2(GL_FILL, 0, partsU, 0, partsV);
	glFrontFace(GL_CCW);


}

Vehicle::Vehicle(){
	angleF = 45;
	angleB = 45;

	ambC[0] = 0.3; ambC[1] = 0.3; ambC[2] = 0.3; ambC[3] = 1;
	difC[0] = 0.3; difC[1] = 0.3; difC[2] = 0.3; difC[3] = 1;
	specC[0] = 0.3; specC[1] = 0.3; specC[2] = 0.3; specC[3] = 1;
	shininessC = 75.f;

	ambV[0] = 0.6; ambV[1] = 0.6; ambV[2] = 0.6; ambV[3] = 1;
	difV[0] = 0.6; difV[1] = 0.6; difV[2] = 0.6; difV[3] = 1;
	specV[0] = 0.6; specV[1] = 0.6; specV[2] = 0.6; specV[3] = 1;
	shininessV = 100.f;

	ambF[0] = 0.4; ambF[1] = 0.4; ambF[2] = 0.4; ambF[3] = 1;
	difF[0] = 0.4; difF[1] = 0.4; difF[2] = 0.4; difF[3] = 1;
	specF[0] = 0.4; specF[1] = 0.4; specF[2] = 0.4; specF[3] = 1;
	shininessF = 75.f;

	metal = new CGFappearance(ambF, difF, specF, shininessF);
	metal->setTexture("textures/gettodachopper.jpg");
	metal2 = new CGFappearance(ambC, difC, specC, shininessC);
	metal2->setTexture("textures/metal.jpg");
	vidro = new CGFappearance(ambV, difV, specV, shininessV);
	vidro->setTexture("textures/vidro.jpg");
	default1 = new CGFappearance(ambF, difF, specF, shininessF);


}
void Vehicle::pes(Cylinder cil){
	glPushMatrix();
	glScaled(0.5, 0.5, 5);
	cil.draw();
	glPopMatrix();

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glScaled(0.25, 0.25, 1);
	glTranslated(0, -5, 0);
	cil.draw();
	glPopMatrix();

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glScaled(0.25, 0.25, 1);
	glTranslated(0, -10, 0);
	cil.draw();
	glPopMatrix();
}
void Vehicle::connector(Cylinder cil){
	glPushMatrix();
	glScaled(0.5, 0.5, 5);
	cil.draw();
	glPopMatrix();

	glPushMatrix();
	glRotated(-90, 0, 1, 0);
	glScaled(0.25, 0.25, 1);
	glTranslated(1.5, 0, -0.15);
	cil.draw();
	glPopMatrix();
}
Patch Vehicle::front(Patch back){
	back.controlPoint.clear();
	vector<GLfloat> temp(3);
	temp[0] = 2.0; temp[1] = 0.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 2.0; temp[1] = 1.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 1.12; temp[1] = 2.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);

	temp[0] = 1.0; temp[1] = 0.0; temp[2] = 6.0;
	back.controlPoint.push_back(temp);
	temp[0] = 1.0; temp[1] = 1.0; temp[2] = 4.0;
	back.controlPoint.push_back(temp);
	temp[0] = 1.0; temp[1] = 2.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);

	temp[0] = 0.0; temp[1] = 0.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 0.0; temp[1] = 1.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 0.82; temp[1] = 2.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	return back;
}
Patch Vehicle::base1(Patch back){
	back.controlPoint.clear();
	vector<GLfloat> temp(3);
	temp[0] = 1.0; temp[1] = 0.0; temp[2] = 3.25;
	back.controlPoint.push_back(temp);
	temp[0] = 0.0; temp[1] = 0.0; temp[2] = 1.25;
	back.controlPoint.push_back(temp);
	temp[0] = 0.0; temp[1] = 0.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);

	temp[0] = 1.0; temp[1] = 0.0; temp[2] = 3.25;
	back.controlPoint.push_back(temp);
	temp[0] = 1.0; temp[1] = 0.0; temp[2] = 1.25;
	back.controlPoint.push_back(temp);
	temp[0] = 1.0; temp[1] = 0.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);

	temp[0] = 1.0; temp[1] = 0.0; temp[2] = 3.25;
	back.controlPoint.push_back(temp);
	temp[0] = 2.0; temp[1] = 0.0; temp[2] = 1.25;
	back.controlPoint.push_back(temp);
	temp[0] = 2.0; temp[1] = 0.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	return back;
}
Patch Vehicle::base2(Patch back){
	back.order = 1;
	back.controlPoint.clear();
	vector<GLfloat> temp(3);
	temp[0] = 0.0; temp[1] = 0.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 0.0; temp[1] = 0.0; temp[2] = -4;
	back.controlPoint.push_back(temp);

	temp[0] = 2.0; temp[1] = 0.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 2.0; temp[1] = 0.0; temp[2] = -4;
	back.controlPoint.push_back(temp);
	return back;
}
Patch Vehicle::backD(Patch back){
	back.order = 2;
	back.controlPoint.clear();
	vector<GLfloat> temp(3);
	temp[0] = 0.0; temp[1] = 0.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);
	temp[0] = 0.0; temp[1] = 1.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);
	temp[0] = 0.82; temp[1] = 2.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);

	temp[0] = 1.0; temp[1] = 0.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);
	temp[0] = 1.0; temp[1] = 1.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);
	temp[0] = 1.0; temp[1] = 2.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);

	temp[0] = 2.0; temp[1] = 0.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);
	temp[0] = 2.0; temp[1] = 1.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);
	temp[0] = 1.18; temp[1] = 2.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);
	return back;
}
Patch Vehicle::roof(Patch back){
	back.order = 2;
	back.controlPoint.clear();
	vector<GLfloat> temp(3);

	temp[0] = 2.0; temp[1] = 0.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 2.0; temp[1] = 0.0; temp[2] = -2.0;
	back.controlPoint.push_back(temp);
	temp[0] = 2.0; temp[1] = 0.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);

	temp[0] = 2.0; temp[1] = 0.5; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 2.0; temp[1] = 0.5; temp[2] = -2.0;
	back.controlPoint.push_back(temp);
	temp[0] = 2.0; temp[1] = 0.5; temp[2] = -4.0;
	back.controlPoint.push_back(temp);

	temp[0] = 1.0; temp[1] = 4.5; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 1.0; temp[1] = 4.5; temp[2] = -2.0;
	back.controlPoint.push_back(temp);
	temp[0] = 1.0; temp[1] = 4.5; temp[2] = -4.0;
	back.controlPoint.push_back(temp);

	temp[0] = 0.0; temp[1] = 0.5; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 0.0; temp[1] = 0.5; temp[2] = -2.0;
	back.controlPoint.push_back(temp);
	temp[0] = 0.0; temp[1] = 0.5; temp[2] = -4.0;
	back.controlPoint.push_back(temp);

	temp[0] = 0.0; temp[1] = 0.0; temp[2] = 0.0;
	back.controlPoint.push_back(temp);
	temp[0] = 0.0; temp[1] = 0.0; temp[2] = -2.0;
	back.controlPoint.push_back(temp);
	temp[0] = 0.0; temp[1] = 0.0; temp[2] = -4.0;
	back.controlPoint.push_back(temp);
	return back;
}
void Vehicle::cube(){
	Rectangle rec1;
	rec1.xy1[0] = 0; rec1.xy1[1] = 0; rec1.xy2[0] = 1; rec1.xy2[1] = 1;
	glPushMatrix();
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
void Vehicle::setangleF(){
	glPushMatrix();
	glRotated(angleF, 0, 1, 0);
	glTranslated(0, 4.75, 0);
	glScaled(1, 0.25, 15);
	cube();
	glPopMatrix();
}
void Vehicle::setangleB(){
	glPushMatrix();
	glTranslated(-1, 2.5, -9.7);
	glRotated(angleB, 1, 0, 0);
	glScaled(0.10, 0.5, 4);
	cube();
	glPopMatrix();
}
void Vehicle::draw(){
	Cylinder cil;
	cil.base = 0.5; cil.height = 1; cil.top = 0.5; cil.slices = 10; cil.stacks = 10;
	metal2->setTextureWrap(GL_CLAMP, GL_CLAMP);
	metal2->apply();
	//Helice
	//Frente
	setangleF();
	//Tras
	setangleB();
	//Pes
	//Esq
	glPushMatrix();
	glTranslated(-1, 0, -1);
	pes(cil);
	glPopMatrix();
	//Dir
	glPushMatrix();
	glTranslated(1, 0, -1);
	pes(cil);
	glPopMatrix();


	glPushMatrix();
	glTranslated(-1.5, 1, 3);
	glScaled(1.5, 1.5, 1.5);

	vidro->setTextureWrap(GL_CLAMP, GL_CLAMP);
	vidro->apply();
	//Body
	Patch back;
	back.compute = "fill"; back.order = 2; back.partsU = 20; back.partsV = 20;
	//Frente
	back = front(back);
	back.draw();

	metal->setTextureWrap(GL_CLAMP, GL_CLAMP);
	metal->apply();
	//Base
	back = base1(back);
	back.draw();
	back = base2(back);
	back.draw();
	//Tras
	back = backD(back);
	back.draw();
	//cima
	back = roof(back);
	back.draw();

	//Conector
	glPushMatrix();
	glTranslated(1, 1, -9);
	connector(cil);
	glPopMatrix();

	metal2->setTextureWrap(GL_CLAMP, GL_CLAMP);
	metal2->apply();
	//Encaixe
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glTranslated(1, 2, 1.5);
	glScaled(0.25, 0.25, 1);
	cil.draw();
	glPopMatrix();

	glPopMatrix();
	default1->apply();
}
void Vehicle::draw(Texture *t){
	Cylinder cil;
	cil.base = 0.5; cil.height = 1; cil.top = 0.5; cil.slices = 10; cil.stacks = 10;
	metal2->setTextureWrap(GL_CLAMP, GL_CLAMP);
	metal2->apply();
	//Helice
	//Frente
	setangleF();
	//Tras
	setangleB();
	//Pes
	//Esq
	glPushMatrix();
	glTranslated(-1, 0, -1);
	pes(cil);
	glPopMatrix();
	//Dir
	glPushMatrix();
	glTranslated(1, 0, -1);
	pes(cil);
	glPopMatrix();


	glPushMatrix();
	glTranslated(-1.5, 1, 3);
	glScaled(1.5, 1.5, 1.5);

	vidro->setTextureWrap(GL_CLAMP, GL_CLAMP);
	vidro->apply();
	//Body
	Patch back;
	back.compute = "fill"; back.order = 2; back.partsU = 20; back.partsV = 20;
	//Frente
	back = front(back);
	back.draw();

	metal->setTextureWrap(GL_CLAMP, GL_CLAMP);
	metal->apply();
	//Base
	back = base1(back);
	back.draw();
	back = base2(back);
	back.draw();
	//Tras
	back = backD(back);
	back.draw();
	//cima
	back = roof(back);
	back.draw();

	//Conector
	glPushMatrix();
	glTranslated(1, 1, -9);
	connector(cil);
	glPopMatrix();

	metal2->setTextureWrap(GL_CLAMP, GL_CLAMP);
	metal2->apply();
	//Encaixe
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glTranslated(1, 2, 1.5);
	glScaled(0.25, 0.25, 1);
	cil.draw();
	glPopMatrix();

	glPopMatrix();
	default1->apply();
}