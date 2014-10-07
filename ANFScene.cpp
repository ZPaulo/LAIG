#include "ANFScene.h"
#include <string>

using namespace std;

ANFScene::ANFScene(char *filename)
{
	// Read XML from file

	doc=new TiXmlDocument( filename );
	bool loadOkay = doc->LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load file '%s'. Error='%s'. Exiting.\n", filename, doc->ErrorDesc() );

		exit( 1 );
	}

	TiXmlElement* anfElement= doc->FirstChildElement( "anf" );

	if (anfElement == NULL)
	{
		printf("Main anf block element not found! Exiting!\n");system("pause");	
		exit(1);
	}


	globalsElement = anfElement->FirstChildElement( "globals" );
	lightsElement = anfElement->FirstChildElement( "lights" );
	camerasElement = anfElement->FirstChildElement( "cameras" );
	textureElement = anfElement->FirstChildElement( "textures" );


	//Globals

	if (globalsElement == NULL)
		printf("Globals block not found!\n");
	else
	{
		printf("Processing globals:\n");

		TiXmlElement* drawingElement=globalsElement->FirstChildElement("drawing");
		if (drawingElement)
		{

			char *valString=NULL;
			float b1,b2,b3,b4;

			valString=(char *) drawingElement->Attribute("background");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				printf("Background values : %f %f %f %f\n", b1, b2, b3,b4);
			}
			else
				printf("Error parsing background");
		}
		else
			printf("drawing not found\n");	

		TiXmlElement* cullingElement=globalsElement->FirstChildElement("culling");
		if (cullingElement)
		{

		}
		else
			printf("culling not found\n");	

		TiXmlElement* lightingElement=globalsElement->FirstChildElement("lighting");
		if (lightingElement)
		{

			char *valString=NULL;
			float b1,b2,b3,b4;

			valString=(char *) lightingElement->Attribute("ambient");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				printf("Ambient values : %f %f %f %f\n", b1, b2, b3,b4);
			}
			else
				printf("Error parsing ambient");
		}
		else
			printf("lighting not found\n");	

	}

	//Cameras
	if (camerasElement == NULL)
		printf("Cameras block not found!\n");
	else
	{
		printf("Processing camerass:\n");

		//TiXmlElement* camera=camerasElement->FirstChildElement();

		//while(camera)
	//	{
			TiXmlElement* perspectiveElement=camerasElement->FirstChildElement("perspective");

			printf("Camera %s\n",camerasElement->Attribute("initial"));
			printf("Perspective %s\n",perspectiveElement->Attribute("id"));

			float near, far, angle, posx, posy, posz, targetx, targety, targetz;
			char *pos=NULL, *target=NULL;

			if(perspectiveElement->QueryFloatAttribute("near",&near)==TIXML_SUCCESS)
				printf("\tNear: %f\n",near);
			if(perspectiveElement->QueryFloatAttribute("far",&far)==TIXML_SUCCESS)
				printf("\tFar: %f\n",far);
			if(perspectiveElement->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
				printf("\tAngle: %f\n",angle);

			pos=(char *) perspectiveElement->Attribute("pos");

			if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
			{
				printf("\tPosition: %f %f %f\n", posx, posy, posz);
			}
			else
				printf("\tError reading position\n");
			target=(char *) perspectiveElement->Attribute("target");

			if(target && sscanf(target,"%f %f %f",&targetx, &targety, &targetz)==3)
			{
				printf("\tTarget: %f %f %f\n", targetx, targety, targetz);
			}
			else
				printf("\tError reading target\n");

			

			TiXmlElement* orthoElement=camerasElement->FirstChildElement("ortho");
			printf("Ortho %s\n",orthoElement->Attribute("id"));

			float direction,near1,far1,left,right,top,bottom;

			if(orthoElement->QueryFloatAttribute("direction",&direction)==TIXML_SUCCESS)
			printf("\tDirection: %f\n",direction);
			if(orthoElement->QueryFloatAttribute("near",&near1)==TIXML_SUCCESS)
			printf("\tNear: %f\n",near1);
			if(orthoElement->QueryFloatAttribute("far",&far1)==TIXML_SUCCESS)
			printf("\tFar: %f\n",far1);
			if(orthoElement->QueryFloatAttribute("left",&left)==TIXML_SUCCESS)
			printf("\tLeft: %f\n",left);
			if(orthoElement->QueryFloatAttribute("right",&right)==TIXML_SUCCESS)
			printf("\tRight: %f\n",right);
			if(orthoElement->QueryFloatAttribute("top",&top)==TIXML_SUCCESS)
			printf("\tTop: %f\n",top);
			if(orthoElement->QueryFloatAttribute("bottom",&bottom)==TIXML_SUCCESS)
			printf("\tBottom: %f\n",bottom);

	//		camera=camera->NextSiblingElement();
	//	}
	}



	//Lights

	if (lightsElement == NULL)
		printf("Lights block not found!\n");
	else
	{
		TiXmlElement *light=lightsElement->FirstChildElement();

		while (light)
		{
			printf("Light - %s\n", light->Attribute("id"));
			printf("\ttype: %s\n",light->Attribute("type"));
			printf("\tenabled: %s\n",light->Attribute("enabled"));
			printf("\tmarker: %s\n",light->Attribute("marker"));

			TiXmlElement* ambient=light->FirstChildElement("ambient");
			TiXmlElement* diffuse=light->FirstChildElement("diffuse");
			TiXmlElement* specular=light->FirstChildElement("specular");

			light=light->NextSiblingElement();

		}
	}

	//Textures

	if(textureElement == NULL)
		printf("Textures block not found!\n");
	else
	{
		TiXmlElement *texture = textureElement->FirstChildElement();
		while(texture)
		{
			printf("Texture - %s\n", texture->Attribute("id"));
			printf("\tfile: %s\n",texture->Attribute("file"));

			texture = texture->NextSiblingElement();
		}
	}

	//Graph

}

ANFScene::~ANFScene()
{
	delete(doc);
}

//-------------------------------------------------------

TiXmlElement *ANFScene::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
	// Searches within descendants of a parent for a light that has an attribute _attr_ (e.g. an id) with the value _val_
	// A more elaborate version of this would rely on XPath expressions
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}