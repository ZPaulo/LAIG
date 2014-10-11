#include "ANFScene.h"
#include <string>
#include <iostream>

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

	parser = new Parser();

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

			parser->globals->drawing.mode = drawingElement->Attribute("mode");
			parser->globals->drawing.shading = drawingElement->Attribute("shading");


			valString=(char *) drawingElement->Attribute("background");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				parser->globals->drawing.background[0] = b1;
				parser->globals->drawing.background[1] = b2;
				parser->globals->drawing.background[2] = b3;
				parser->globals->drawing.background[3] = b4;
			}
			else
				printf("Error parsing background");
		}
		else
			printf("drawing not found\n");	

		TiXmlElement* cullingElement=globalsElement->FirstChildElement("culling");
		if (cullingElement)
		{
			parser->globals->culling.face = cullingElement->Attribute("face");
			parser->globals->culling.order = cullingElement->Attribute("order");
		}
		else
			printf("culling not found\n");	

		TiXmlElement* lightingElement=globalsElement->FirstChildElement("lighting");
		if (lightingElement)
		{

			parser->globals->lighting.local = lightingElement->Attribute("local");
			parser->globals->lighting.doublesdd = lightingElement->Attribute("doublesided");
			parser->globals->lighting.enabled = lightingElement->Attribute("enabled");

			char *valString=NULL;
			float b1,b2,b3,b4;

			valString=(char *) lightingElement->Attribute("ambient");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				parser->globals->lighting.ambient[0] = b1;
				parser->globals->lighting.ambient[1] = b2;
				parser->globals->lighting.ambient[2] = b3;
				parser->globals->lighting.ambient[3] = b4;
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
		CPerspective cPer;
		TiXmlElement* ortho=camerasElement->FirstChildElement("ortho");
		COrtho cOrt;

		parser->initCam = camerasElement->Attribute("initial");
		parser->activeCam = parser->initCam;

		while(perspective){
			cPer.id = perspective->Attribute("id");

			float near, far, angle, posx, posy, posz, targetx, targety, targetz;
			char *pos=NULL, *target=NULL;

			if(perspective->QueryFloatAttribute("near",&near)==TIXML_SUCCESS)
				cPer.near = near;
			if(perspective->QueryFloatAttribute("far",&far)==TIXML_SUCCESS)
				cPer.far = far;
			if(perspective->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
				cPer.angle = angle;

			pos=(char *) perspective->Attribute("pos");

			if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
			{
				cPer.pos[0] = posx;
				cPer.pos[1] = posy;
				cPer.pos[2] = posz;
			}
			else
				printf("\tError reading position\n");
			target=(char *) perspective->Attribute("target");

			if(target && sscanf(target,"%f %f %f",&targetx, &targety, &targetz)==3)
			{
				cPer.target[0] = targetx;
				cPer.target[1] = targety;
				cPer.target[2] = targetz;
			}
			else
				printf("\tError reading target\n");

			parser->cameras[cPer.id] = cPer;
			perspective=perspective->NextSiblingElement("perspective");
		}



		float direction,near1,far1,left,right,top,bottom;
		while(ortho)
		{
			cOrt.id = ortho->Attribute("id");

			if(ortho->QueryFloatAttribute("direction",&direction)==TIXML_SUCCESS)
				cOrt.direction = direction;

			if(ortho->QueryFloatAttribute("near",&near1)==TIXML_SUCCESS)
				cOrt.near = near1;

			if(ortho->QueryFloatAttribute("far",&far1)==TIXML_SUCCESS)
				cOrt.far = far1;

			if(ortho->QueryFloatAttribute("left",&left)==TIXML_SUCCESS)
				cOrt.left = left;

			if(ortho->QueryFloatAttribute("right",&right)==TIXML_SUCCESS)
				cOrt.right = right;

			if(ortho->QueryFloatAttribute("top",&top)==TIXML_SUCCESS)
				cOrt.top = top;

			if(ortho->QueryFloatAttribute("bottom",&bottom)==TIXML_SUCCESS)
				cOrt.bottom = bottom;

			parser->cameras[cOrt.id] = cOrt;
			ortho=ortho->NextSiblingElement("ortho");
		}
	}


	//Lights

	if (lightsElement == NULL)
		printf("Lights block not found!\n");
	else
	{
		printf("Processing Lights:\n");
		TiXmlElement *light=lightsElement->FirstChildElement();

		while (light)
		{
			Light lt;
			lt.id = light->Attribute("id");
			lt.type = light->Attribute("type");
			lt.enabled =light->Attribute("enabled");
			lt.marker = light->Attribute("marker");
			if(lt.type == "spot")
			{
				float angle, exponent,targetx, targety, targetz;
				char *target=NULL;
				if(light->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
					lt.angle = angle;

				if(light->QueryFloatAttribute("exponent",&exponent)==TIXML_SUCCESS)
					lt.exponent = exponent;

				target=(char *) light->Attribute("target");

				if(target && sscanf(target,"%f %f %f",&targetx, &targety, &targetz)==3)
				{
					lt.target[0] = targetx;
					lt.target[1] = targety;
					lt.target[2] = targetz;
				}
				else
					printf("\tError reading target\n");
			}

			char *valString=NULL;
			float b1,b2,b3,b4;
			TiXmlElement *component = light->FirstChildElement();

			valString=(char *) component->Attribute("value");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				lt.ambient[0] = b1;
				lt.ambient[1] = b2;
				lt.ambient[2] = b3;
				lt.ambient[3] = b4;
			}
			else
				printf("Error parsing ambient\n");

			component= component->NextSiblingElement();

			valString=(char *) component->Attribute("value");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				lt.diffuse[0] = b1;
				lt.diffuse[1] = b2;
				lt.diffuse[2] = b3;
				lt.diffuse[3] = b4;
			}
			else
				printf("Error parsing diffuse\n");

			component = component->NextSiblingElement();

			valString=(char *) component->Attribute("value");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				lt.specular[0] = b1;
				lt.specular[1] = b2;
				lt.specular[2] = b3;
				lt.specular[3] = b4;
			}
			else
				printf("Error parsing specular\n");


			parser->lights[lt.id] = lt;
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
		parser->graph->rootID = graphElement->Attribute("rootid");

		if(	node == NULL)
			printf("Node block not found!\n");
		else
		{
			while (node)
			{
				Node* pNode = new Node();
				pNode->id = node->Attribute("id");
				TiXmlElement *transforms = node->FirstChildElement("transforms");
				if(transforms == NULL)
				{
					printf("Transforms block not found!\n");
					break;
				}
				else
				{
					TiXmlElement *transform = transforms->FirstChildElement("transform");

					while(transform)
					{
						Transform *tr = new Transform();
						if((string) transform->Attribute("type") == "translate")
						{

							tr->type = "translate";
							char *pos=NULL;
							float posx,posy,posz;
							pos=(char *) transform->Attribute("to");

							if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
							{
								tr->to[0] = posx;
								tr->to[1] = posy;
								tr->to[2] = posz;
							}
							else
								printf("\tError reading position\n");
						}
						if((string) transform->Attribute("type") == "rotate")
						{
							tr->type = "rotate";

							tr->axis = transform->Attribute("axis");
							float angle;
							if(transform->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
								tr->angle = angle;
						}

						if((string) transform->Attribute("type") == "scale")
						{
							tr->type = "scale";
							char *pos=NULL;
							float posx,posy,posz;
							pos=(char *) transform->Attribute("factor");

							if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
							{
								tr->factor[0] = posx;
								tr->factor[1] = posy;
								tr->factor[2] = posz;
							}
							else
								printf("\tError reading factor\n");
						}


						pNode->tranforms.push_back(tr);
						transform = transform->NextSiblingElement();
					}
				}

				TiXmlElement *appearance = node->FirstChildElement("appearanceref");
				if(appearance->Attribute("id") != "inherit")
				{
					pNode->apperance = &(parser->appearances[appearance->Attribute("id")]);
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
						Rectangle* rect = new Rectangle();
						rect->name = "rectangle";

						char *pos=NULL;
						float pos1,pos2;
						pos=(char *) rectangle->Attribute("xy1");

						if(pos && sscanf(pos,"%f %f",&pos1, &pos2)==2)
						{
							rect->xy1[0] = pos1;
							rect->xy1[1] = pos2;
						}
						else
							printf("\tError reading rectangle\n");

						pos=(char *) rectangle->Attribute("xy2");

						if(pos && sscanf(pos,"%f %f",&pos1, &pos2)==2)
						{
							rect->xy2[0] = pos1;
							rect->xy2[1] = pos2;
						}
						else
							printf("\tError reading rectangle\n");

						pNode->primitives.push_back(rect);
						rectangle=rectangle->NextSiblingElement("rectangle");
					}
					while(triangle){

						Triangle* tri = new Triangle();
						tri->name = "triangle";

						char *pos=NULL;
						float pos1,pos2, pos3;
						pos=(char *) triangle->Attribute("xyz1");

						if(pos && sscanf(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
						{
							tri->xyz1[0] = pos1;
							tri->xyz1[1] = pos2;
							tri->xyz1[2] = pos3;
						}
						else
							printf("\tError reading triangle\n");

						pos=(char *) triangle->Attribute("xyz2");

						if(pos && sscanf(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
						{
							tri->xyz2[0] = pos1;
							tri->xyz2[1] = pos2;
							tri->xyz2[2] = pos3;
						}
						else
							printf("\tError reading triangle\n");

						pos=(char *) triangle->Attribute("xyz3");

						if(pos && sscanf(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
						{
							tri->xyz3[0] = pos1;
							tri->xyz3[1] = pos2;
							tri->xyz3[2] = pos3;
						}
						else
							printf("\tError reading triangle\n");

						pNode->primitives.push_back(tri);

						triangle=triangle->NextSiblingElement("triangle");
					}
					while(cylinder){
						Cylinder* cyl = new Cylinder(); 
						cyl->name = "cylinder";

						float base,top,height;
						int slices, stacks;

						if(cylinder->QueryFloatAttribute("base",&base)==TIXML_SUCCESS)
							cyl->base = base;
						if(cylinder->QueryFloatAttribute("top",&top)==TIXML_SUCCESS)
							cyl->top = top;
						if(cylinder->QueryFloatAttribute("height",&height)==TIXML_SUCCESS)
							cyl->height = height;
						if(cylinder->QueryIntAttribute("slices",&slices)==TIXML_SUCCESS)
							cyl->slices = slices;
						if(cylinder->QueryIntAttribute("stacks",&stacks)==TIXML_SUCCESS)
							cyl->stacks = stacks;

						pNode->primitives.push_back(cyl);
						cylinder=cylinder->NextSiblingElement("cylinder");
					}
					while(sphere){
						Sphere* sph = new Sphere();
						sph->name = "sphere";

						float radius;
						int slices, stacks;

						if(sphere->QueryFloatAttribute("radius",&radius)==TIXML_SUCCESS)
							sph->radius = radius;
						if(sphere->QueryIntAttribute("slices",&slices)==TIXML_SUCCESS)
							sph->slices = slices;
						if(sphere->QueryIntAttribute("stacks",&stacks)==TIXML_SUCCESS)
							sph->stacks = stacks;

						pNode->primitives.push_back(sph);
						sphere=sphere->NextSiblingElement("sphere");
					}
					while(torus){

						Torus* tor = new Torus();
						tor->name = "torus";

						float inner, outer;
						int slices, loops;

						if(torus->QueryFloatAttribute("inner",&inner)==TIXML_SUCCESS)
							tor->inner = inner;
						if(torus->QueryFloatAttribute("outer",&outer)==TIXML_SUCCESS)
							tor->outer = outer;
						if(torus->QueryIntAttribute("slices",&slices)==TIXML_SUCCESS)
							tor->slices = slices;
						if(torus->QueryIntAttribute("loops",&loops)==TIXML_SUCCESS)
							tor->loops = loops;

						pNode->primitives.push_back(tor);
						torus=torus->NextSiblingElement("torus");
					}

				}

				TiXmlElement *descendants = node->FirstChildElement("descendants");
				if(descendants !=NULL)
				{
					TiXmlElement *nodeRef = descendants->FirstChildElement();
					while(nodeRef)
					{
						pNode->descendants[nodeRef->Attribute("id")] = parser->graph->nodes[nodeRef->Attribute("id")];

						nodeRef = nodeRef->NextSiblingElement();
					}
				}
				parser->graph->nodes[pNode->id] = pNode;
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