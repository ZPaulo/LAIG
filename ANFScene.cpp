#include "ANFScene.h"
#include <string>

using namespace std;

ANFScene::ANFScene(char *filename)
{
	// Read ANF  from file

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
	appearancesElement= anfElement->FirstChildElement("appearances");
	graphElement = anfElement->FirstChildElement("graph");


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

		TiXmlElement* perspective=camerasElement->FirstChildElement("perspective");
		TiXmlElement* ortho=camerasElement->FirstChildElement("ortho");

		printf("Camera %s\n",camerasElement->Attribute("initial"));

		while(perspective){
			printf("Perspective %s\n",perspective->Attribute("id"));

			float near, far, angle, posx, posy, posz, targetx, targety, targetz;
			char *pos=NULL, *target=NULL;

			if(perspective->QueryFloatAttribute("near",&near)==TIXML_SUCCESS)
				printf("\tNear: %f\n",near);
			if(perspective->QueryFloatAttribute("far",&far)==TIXML_SUCCESS)
				printf("\tFar: %f\n",far);
			if(perspective->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
				printf("\tAngle: %f\n",angle);

			pos=(char *) perspective->Attribute("pos");

			if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
			{
				printf("\tPosition: %f %f %f\n", posx, posy, posz);
			}
			else
				printf("\tError reading position\n");
			target=(char *) perspective->Attribute("target");

			if(target && sscanf(target,"%f %f %f",&targetx, &targety, &targetz)==3)
			{
				printf("\tTarget: %f %f %f\n", targetx, targety, targetz);
			}
			else
				printf("\tError reading target\n");

			perspective=perspective->NextSiblingElement("perspective");
			}
			
			

			float direction,near1,far1,left,right,top,bottom;
			while(ortho){
			printf("Ortho %s\n",ortho->Attribute("id"));
			if(ortho->QueryFloatAttribute("direction",&direction)==TIXML_SUCCESS)
			printf("\tDirection: %f\n",direction);
			if(ortho->QueryFloatAttribute("near",&near1)==TIXML_SUCCESS)
			printf("\tNear: %f\n",near1);
			if(ortho->QueryFloatAttribute("far",&far1)==TIXML_SUCCESS)
			printf("\tFar: %f\n",far1);
			if(ortho->QueryFloatAttribute("left",&left)==TIXML_SUCCESS)
			printf("\tLeft: %f\n",left);
			if(ortho->QueryFloatAttribute("right",&right)==TIXML_SUCCESS)
			printf("\tRight: %f\n",right);
			if(ortho->QueryFloatAttribute("top",&top)==TIXML_SUCCESS)
			printf("\tTop: %f\n",top);
			if(ortho->QueryFloatAttribute("bottom",&bottom)==TIXML_SUCCESS)
			printf("\tBottom: %f\n",bottom);

		
			ortho=ortho->NextSiblingElement("ortho");
			}
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

	//appearances
	float shininess;
	if (appearancesElement==NULL)
		printf("Appearances Block not found!\n");
	else
	{
		TiXmlElement *appearance=appearancesElement->FirstChildElement();
		while (appearance)
		{
			printf("Appearance - %s\n", appearance->Attribute("id"));
			appearance->QueryFloatAttribute("shininess",&shininess);
			printf("\tShininess: %f\n", shininess);
			printf("\tTextureRef: %s\n", appearance->Attribute("textureref"));

			appearance=appearance->NextSiblingElement();
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

	if (graphElement == NULL)
		printf("Graph block not found!\n");
	else
	{

		TiXmlElement *node=graphElement->FirstChildElement();
		if(	node == NULL)
			printf("Node block not found!\n");
		else
		{
			while (node)
			{
				TiXmlElement *transforms = node->FirstChildElement("transforms");
				if(transforms == NULL)
				{
					printf("Transforms block not found!\n");
					break;
				}
				else
				{
					TiXmlElement *transform = transforms->FirstChildElement();

					while(transform)
					{
						printf("Transformation: %s\n",transform->Attribute("type"));

						transform = transform->NextSiblingElement();
					}
				}

				TiXmlElement *appearance = node->FirstChildElement("appearanceref");
				if(appearance->Attribute("id") == "inherit")
				{
					appearance = node->Parent.FirstChildElement("appearanceref");
				}

				TiXmlElement *primitives = node->FirstChildElement("primitives");
				if(primitives == NULL)
				{
					printf("Primitives block not found!\n");
					break;
				}
				else
				{
					TiXmlElement *rectangle = primitives->FirstChildElement("rectangle");
					TiXmlElement *triangle = primitives->FirstChildElement("triangle");
					TiXmlElement *cylinder = primitives->FirstChildElement("cylinder");
					TiXmlElement *sphere = primitives->FirstChildElement("sphere");
					TiXmlElement *torus = primitives->FirstChildElement("torus");

					while(rectangle){
						printf("Rectangle: \n");
						rectangle=rectangle->NextSiblingElement("rectangle");
					}
					while(triangle){
						printf("Triangle: \n");
						triangle=triangle->NextSiblingElement("triangle");
					}
					while(cylinder){
						printf("Cylinder: \n");
						cylinder=cylinder->NextSiblingElement("cylinder");
					}
					while(sphere){
						printf("Sphere: \n");
						sphere=sphere->NextSiblingElement("sphere");
					}
					while(torus){
						printf("Torus: \n");
						torus=torus->NextSiblingElement("torus");
					}

				}
				
				TiXmlElement *descendants = node->FirstChildElement("descendants");

				TiXmlElement *descendant = descendants->FirstChildElement();
				while(descendant)
				{
					printf("Descendant id: %s", descendant->Attribute("id"));

					descendant = descendant->NextSiblingElement();
				}

				node = node->NextSiblingElement();
			}
		}
	}

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