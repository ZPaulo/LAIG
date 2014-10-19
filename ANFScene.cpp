#include "ANFScene.h"
#include <string>
#include <iostream>
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <math.h>

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

	parser;

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
			string s = drawingElement->Attribute("mode");
			if(s == "fill")
				parser.globals->drawing.mode = 0;
			else if(s == "line")
				parser.globals->drawing.mode = 1;
			else if(s=="point")
				parser.globals->drawing.mode = 2;
			else{
				printf("Error reading drawing mode, assuming fill\n");
				parser.globals->drawing.mode = 0;
			}
			
			parser.globals->drawing.shading = drawingElement->Attribute("shading");
			if(parser.globals->drawing.shading !="flat" && parser.globals->drawing.shading !="gouraud"){
				printf("Error reading drawing shading, assuming flat\n");
				parser.globals->drawing.shading ="flat";
			}

			valString=(char *) drawingElement->Attribute("background");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				parser.globals->drawing.background[0] = b1;
				parser.globals->drawing.background[1] = b2;
				parser.globals->drawing.background[2] = b3;
				parser.globals->drawing.background[3] = b4;
			}
			else{
				printf("Error parsing background, assuming 0 0 0 0\n");
				parser.globals->drawing.background[0] = 0;
				parser.globals->drawing.background[1] = 0;
				parser.globals->drawing.background[2] = 0;
				parser.globals->drawing.background[3] = 0;
			}
		}
		else
			printf("drawing not found\n");	

		TiXmlElement* cullingElement=globalsElement->FirstChildElement("culling");
		if (cullingElement)
		{
			parser.globals->culling.face = cullingElement->Attribute("face");
			if(parser.globals->culling.face !="none" && parser.globals->culling.face !="back" && parser.globals->culling.face !="front" && parser.globals->culling.face !="both"){
				printf("Error reading culling face, assuming none\n");
				parser.globals->culling.face ="none";
			}

			parser.globals->culling.order = cullingElement->Attribute("order");
			if(parser.globals->culling.order != "ccw" && parser.globals->culling.order != "cw"){
				printf("Error reading culling order, assuming ccw\n");
				parser.globals->culling.face ="ccw";
			}
		}
		else
			printf("culling not found\n");	

		TiXmlElement* lightingElement=globalsElement->FirstChildElement("lighting");
		if (lightingElement)
		{

			if((string)lightingElement->Attribute("local") == "true")
				parser.globals->lighting.local = true;
			else
				parser.globals->lighting.local = false;

			if((string) lightingElement->Attribute("doublesided") == "true")
				parser.globals->lighting.doublesdd = true;
			else
				parser.globals->lighting.doublesdd = false;

			if((string) lightingElement->Attribute("enabled") == "true")
				parser.globals->lighting.enabled = true;
			else
				parser.globals->lighting.enabled = false;

			

			char *valString=NULL;
			float b1,b2,b3,b4;

			valString=(char *) lightingElement->Attribute("ambient");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				parser.globals->lighting.ambient[0] = b1;
				parser.globals->lighting.ambient[1] = b2;
				parser.globals->lighting.ambient[2] = b3;
				parser.globals->lighting.ambient[3] = b4;
			}
			else{
				printf("Error parsing ambient, assuming 0 0 0 0");
				parser.globals->lighting.ambient[0] = 0;
				parser.globals->lighting.ambient[1] = 0;
				parser.globals->lighting.ambient[2] = 0;
				parser.globals->lighting.ambient[3] = 0;
			}
		}
		else
			printf("lighting not found\n");	

	}

	//Cameras
	if (camerasElement == NULL)
		printf("Cameras block not found!\n");
	else
	{
		printf("Processing cameras:\n");

		TiXmlElement* perspective=camerasElement->FirstChildElement("perspective");

		TiXmlElement* ortho=camerasElement->FirstChildElement("ortho");

		parser.initCam = camerasElement->Attribute("initial");
		int i=0;
		while(perspective){
			CPerspective *cPer = new CPerspective();

			cPer->id = perspective->Attribute("id");
			if(cPer->id == "")
				cPer->id="cam_pers_"+i;
			i++;
			float near, far, angle, posx, posy, posz, targetx, targety, targetz;
			char *pos=NULL, *target=NULL;

			if(perspective->QueryFloatAttribute("near",&near)==TIXML_SUCCESS)
				cPer->near = near;
			else{
				printf("Error reading camara perspective near param\n");
				cPer->near = 0.0;
			}
			if(perspective->QueryFloatAttribute("far",&far)==TIXML_SUCCESS)
				cPer->far = far;
			else{
				printf("Error reading camara perspective far param\n");
				cPer->far = 100.0;
			}
			if(perspective->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
				cPer->angle = angle;
			else{
				printf("Error reading camara perspective angle param\n");
				cPer->angle = 90.0;
			}
			pos=(char *) perspective->Attribute("pos");

			if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
			{
				cPer->pos[0] = posx;
				cPer->pos[1] = posy;
				cPer->pos[2] = posz;
			}
			else{
				printf("\tError reading position, assuming 0 0 0\n");
				cPer->pos[0] = 0;
				cPer->pos[1] = 0;
				cPer->pos[2] = 0;
			}

			target=(char *) perspective->Attribute("target");
			if(target && sscanf(target,"%f %f %f",&targetx, &targety, &targetz)==3)
			{
				cPer->target[0] = targetx;
				cPer->target[1] = targety;
				cPer->target[2] = targetz;
			}
			else{
				printf("\tError reading target, assuming 0 0 0\n");
				cPer->target[0] = 0;
				cPer->target[1] = 0;
				cPer->target[2] = 0;
			}

			parser.cameras.push_back(cPer);
			perspective=perspective->NextSiblingElement("perspective");
		}



		float direction,near1,far1,left,right,top,bottom;
		i=0;
		while(ortho)
		{
			COrtho *cOrt = new COrtho();
			cOrt->id = ortho->Attribute("id");
			if(cOrt->id=="")
				cOrt->id="cam_orth_"+i;
			i++;

			cOrt->direction = ortho->Attribute("direction");
			if(ortho->QueryFloatAttribute("near",&near1)==TIXML_SUCCESS)
				cOrt->near = near1;
			else{
				printf("Error reading camara ortho near param\n");
				cOrt->near = 0;
			}
			if(ortho->QueryFloatAttribute("far",&far1)==TIXML_SUCCESS)
				cOrt->far = far1;
			else{
				printf("Error reading camara ortho far param\n");
				cOrt->far = 0;
			}
			if(ortho->QueryFloatAttribute("left",&left)==TIXML_SUCCESS)
				cOrt->left = left;
			else{
				printf("Error reading camara ortho left param\n");
				cOrt->left = 0;
			}
			if(ortho->QueryFloatAttribute("right",&right)==TIXML_SUCCESS)
				cOrt->right = right;
			else{
				printf("Error reading camara ortho right param\n");
				cOrt->right = 0;
			}
			if(ortho->QueryFloatAttribute("top",&top)==TIXML_SUCCESS)
				cOrt->top = top;
			else{
				printf("Error reading camara ortho top param\n");
				cOrt->right = 0;
			}
			if(ortho->QueryFloatAttribute("bottom",&bottom)==TIXML_SUCCESS)
				cOrt->bottom = bottom;
			else{
				printf("Error reading camara ortho bottom param\n");
				cOrt->bottom = 0;
			}

			parser.cameras.push_back(cOrt);
			ortho=ortho->NextSiblingElement("ortho");
		}
		if(parser.initCam==""&&parser.cameras.size()>0)
			parser.initCam=parser.cameras[0]->id;
		
		for(int i = 0; i < parser.cameras.size();i++)
		{
			if(parser.cameras[i]->id == parser.initCam)
			{
				parser.activeCam = i;
				break;
			}
		}
	
	}


	//Lights
	int i=0;
	if (lightsElement == NULL)
		printf("Lights block not found!\n");
	else
	{
		printf("Processing Lights:\n");
		TiXmlElement *light=lightsElement->FirstChildElement();

		while (light)
		{
			Light *lt = new Light();
			lt->id = light->Attribute("id");
			if(lt->id=="")
				lt->id="light_"+i;
			i++;
			lt->type = light->Attribute("type");
			if(lt->type != "spot" && lt->type !="omni")
			{
				printf("Error reading light type, assuming omni\n");
				lt->type="omni";
			}
			if((string)light->Attribute("enabled") == "true")
				lt->enabled = true;
			else
				lt->enabled = false;

			if((string)light->Attribute("marker") == "true")
				lt->marker =true;
			else
				lt->marker = false;

			char *pos=NULL;
			float pos1,pos2, pos3;
			pos=(char *) light->Attribute("pos");

			if(pos && sscanf(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
			{
				lt->pos[0] = pos1;
				lt->pos[1] = pos2;
				lt->pos[2] = pos3;
			}
			else{
				printf("\tError reading light pos, assuming 0 0 0\n");
				lt->pos[0] = 0;
				lt->pos[1] = 0;
				lt->pos[2] = 0;
			}

			if(lt->type == "spot")
			{
				float angle, exponent,targetx, targety, targetz;
				char *target=NULL;
				if(light->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
					lt->angle = angle;
				else{
					printf("Error reading light spot angle, assuming 90\n");
					lt->angle=90;
				}
				if(light->QueryFloatAttribute("exponent",&exponent)==TIXML_SUCCESS)
					lt->exponent = exponent;
				else{
					printf("Error reading light spot exponent, assuming 1.0\n");
					lt->exponent=1.0;
				}
				target=(char *) light->Attribute("target");

				if(target && sscanf(target,"%f %f %f",&targetx, &targety, &targetz)==3)
				{
					lt->target[0] = targetx;
					lt->target[1] = targety;
					lt->target[2] = targetz;
				}
				else{
					printf("\tError reading light spot target, assuming 0 0 0\n");
					lt->target[0] = 0;
					lt->target[1] = 0;
					lt->target[2] = 0;
				}
			}

			char *valString=NULL;
			float b1,b2,b3,b4;
			TiXmlElement *component = light->FirstChildElement();

			valString=(char *) component->Attribute("value");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				lt->ambient[0] = b1;
				lt->ambient[1] = b2;
				lt->ambient[2] = b3;
				lt->ambient[3] = b4;
			}
			else{
				printf("Error parsing light ambient parameter, assuming 0 0 0 0\n");
				lt->ambient[0] = 0;
				lt->ambient[1] = 0;
				lt->ambient[2] = 0;
				lt->ambient[3] = 0;
			}

			component= component->NextSiblingElement();

			valString=(char *) component->Attribute("value");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				lt->diffuse[0] = b1;
				lt->diffuse[1] = b2;
				lt->diffuse[2] = b3;
				lt->diffuse[3] = b4;
			}
			else{
				printf("Error parsing light diffuse parameter, assuming 0 0 0 0\n");
				lt->diffuse[0] = 0;
				lt->diffuse[1] = 0;
				lt->diffuse[2] = 0;
				lt->diffuse[3] = 0;
			}

			component = component->NextSiblingElement();

			valString=(char *) component->Attribute("value");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				lt->specular[0] = b1;
				lt->specular[1] = b2;
				lt->specular[2] = b3;
				lt->specular[3] = b4;
			}
			else{
				printf("Error parsing light specular parameter, assuming 0 0 0 0\n");
				lt->specular[0] = 0;
				lt->specular[1] = 0;
				lt->specular[2] = 0;
				lt->specular[3] = 0;
			}


			parser.lights.push_back(lt);
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
			float texS,texT;
			Texture *tex = new Texture();
			tex->id = texture->Attribute("id");
			tex->file = texture->Attribute("file");
			texture->QueryFloatAttribute("texlength_s",&texS);
			tex->texLengthS = texS;
			texture->QueryFloatAttribute("texlength_t",&texT);
			tex->texLengthT = texT;
			parser.textures[tex->id] = tex;
			texture = texture->NextSiblingElement();
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
			string id;
			Appearance *app = new Appearance();
			id =  appearance->Attribute("id");
			appearance->QueryFloatAttribute("shininess",&shininess);
			app->shininness = shininess;
			if(appearance->Attribute("textureref")) 
				if(appearance->Attribute("textureref")!= "")
					app->textureRef = appearance->Attribute("textureref");


			char *valString=NULL;
			float b1,b2,b3,b4;
			TiXmlElement *component = appearance->FirstChildElement();

			valString=(char *) component->Attribute("value");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				app->ambient[0] = b1;
				app->ambient[1] = b2;
				app->ambient[2] = b3;
				app->ambient[3] = b4;
			}
			else
				printf("Error parsing ambient\n");

			component= component->NextSiblingElement();

			valString=(char *) component->Attribute("value");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				app->diffuse[0] = b1;
				app->diffuse[1] = b2;
				app->diffuse[2] = b3;
				app->diffuse[3] = b4;
			}
			else
				printf("Error parsing diffuse\n");

			component = component->NextSiblingElement();

			valString=(char *) component->Attribute("value");

			if(valString && sscanf(valString,"%f %f %f %f",&b1, &b2, &b3, &b4)==4)
			{
				app->specular[0] = b1;
				app->specular[1] = b2;
				app->specular[2] = b3;
				app->specular[3] = b4;
			}
			else
				printf("Error parsing specular\n");

			app->isTexApp = false;
			CGFappearance *appC = new CGFappearance(app->ambient,app->diffuse,app->specular,app->shininness);
			if(app->textureRef != "")
				if(parser.textures[app->textureRef])
				{

					if (FILE *file = fopen(parser.textures[app->textureRef]->file.c_str(), "r")) 
					{
						fclose(file);
						appC->setTexture(parser.textures[app->textureRef]->file);
						appC->setTextureWrap(GL_REPEAT,GL_REPEAT);	
						app->isTexApp = true;
					} 
				}
				else 
					app->textureRef = "";



			app->appCGF = appC;
			parser.appearances[id] = app;
			appearance=appearance->NextSiblingElement();

		}
	}

	//Graph

	if (graphElement == NULL)
		printf("Graph block not found!\n");
	else
	{

		TiXmlElement *node=graphElement->FirstChildElement();
		parser.graph->rootID = graphElement->Attribute("rootid");

		if(	node == NULL)
			printf("Node block not found!\n");
		else
		{
			while (node)
			{
				Node* pNode = new Node();
				pNode->id = node->Attribute("id");
				TiXmlElement *transforms = node->FirstChildElement("transforms");
				glLoadIdentity();
				glGetFloatv(GL_MODELVIEW_MATRIX,pNode->matrix);
				if(transforms == NULL)
				{
					printf("Transforms block not found!\n");
				}
				else
				{
					TiXmlElement *transform = transforms->FirstChildElement("transform");
					while(transform)
					{
						if((string) transform->Attribute("type") == "translate")
						{

							char *pos=NULL;
							float posx,posy,posz;
							pos=(char *) transform->Attribute("to");

							if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
							{
								glTranslatef(posx,posy,posz);
							}
							else
								printf("\tError reading position\n");
						}
						if((string) transform->Attribute("type") == "rotate")
						{
							string axis = (char*) transform->Attribute("axis");
							float angle;
							if(transform->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
								glRotatef(angle, axis == "x", axis == "y", axis == "z");
						}

						if((string) transform->Attribute("type") == "scale")
						{
							char *pos=NULL;
							float posx,posy,posz;
							pos=(char *) transform->Attribute("factor");

							if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
							{
								glScalef(posx,posy,posz);
							}
							else
								printf("\tError reading factor\n");
						}


						glGetFloatv(GL_MODELVIEW_MATRIX,pNode->matrix);
						transform = transform->NextSiblingElement();
					}
				}

				TiXmlElement *appearance = node->FirstChildElement("appearanceref");

				if(appearance)
					pNode->apperanceRef = appearance->Attribute("id");
				else 
					pNode->apperanceRef = "";

				TiXmlElement *primitives = node->FirstChildElement("primitives");
				if(primitives == NULL)
				{
					printf("Primitives block not found!\n");
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
						pNode->descendants.push_back(nodeRef->Attribute("id"));

						nodeRef = nodeRef->NextSiblingElement();
					}
				}
				parser.graph->nodes[pNode->id] = pNode;
				node = node->NextSiblingElement();

			}
		}
	}


}
void ANFScene::activateLight(int id,bool enable1){
	parser.lights[id]->enabled = enable1;
}

ANFScene::~ANFScene()
{
	delete(doc);
}

//-------------------------------------------------------


void ANFScene::init() 
{

	float globalAmbientLight[4];
	for(int i = 0; i < 4 ;i++)
		globalAmbientLight[i] = parser.globals->lighting.ambient[i];

	if(parser.globals->lighting.enabled)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if(parser.globals->lighting.doublesdd)
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	if(parser.globals->lighting.local)
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);



	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);  


	if(parser.globals->drawing.shading == "flat")
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);



	glClearColor(parser.globals->drawing.background[0], parser.globals->drawing.background[1],
		parser.globals->drawing.background[2],parser.globals->drawing.background[3]);

	//Globals culling
	glEnable(GL_CULL_FACE);

	if(parser.globals->culling.face == "none")
		glDisable(GL_CULL_FACE);
	else if (parser.globals->culling.face == "front")
		glCullFace(GL_FRONT);
	else if (parser.globals->culling.face == "back")
		glCullFace(GL_BACK);
	else
		glCullFace(GL_FRONT_AND_BACK);

	if(parser.globals->culling.order == "cw")
		glFrontFace(GL_CW);
	else
		glFrontFace(GL_CCW);

	int id[8];
	id[0] = GL_LIGHT0;
	id[1] = GL_LIGHT1;
	id[2] = GL_LIGHT2;
	id[3] = GL_LIGHT3;
	id[4] = GL_LIGHT4;
	id[5] = GL_LIGHT5;
	id[6] = GL_LIGHT6;
	id[7] = GL_LIGHT7;


	for(unsigned int i=0;i<parser.lights.size() && i<8;i++)
	{


		CGFlight* light;
		Light* recentlight = parser.lights[i];
		float pos[4],dir[3], unit;
		for(int i = 0; i< 3; i++)
		{
			pos[i] = recentlight->pos[i];
			dir[i]= recentlight->target[i] - pos[i];
		}
		pos[3] = 1;
		unit = sqrt(dir[0]*dir[0]+dir[1]*dir[1]+dir[2]*dir[2]);
		for(int i = 0; i< 3; i++)
		{
			dir[i] = dir[i] / unit;
		}
		if(recentlight->type==("spot")){
			glLightf(id[i],GL_SPOT_CUTOFF,recentlight->angle);
			glLightf(id[i],GL_SPOT_EXPONENT,recentlight->exponent);
			glLightfv(id[i],GL_SPOT_DIRECTION,recentlight->target);
		}

		light = new CGFlight(id[i], pos);
		light->setAmbient(recentlight->ambient);
		light->setSpecular(recentlight->specular);
		light->setDiffuse(recentlight->diffuse);

		lightsV.push_back(light);
	}



	// Uncomment below to enable normalization of lighting normal vectors
	glEnable (GL_NORMALIZE);
	glEnable (GL_TEXTURE_2D);



}




void ANFScene::display() 
{

	// ---- BEGIN Background, camera and axis setup

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if(parser.activeCam >= parser.cameras.size())
		CGFscene::activeCamera->applyView();
	else
		parser.cameras[parser.activeCam]->apply();

	CGFapplication::activeApp->forceRefresh();

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//parser.cameras[parser.activeCam]->camera
	// Apply transformations corresponding to the camera position relative to the origin
	//CGFscene::activeCamera->applyView();



	for(unsigned int i=0;i<parser.lights.size() && i<8;i++)
	{
		if(parser.lights[i]->marker)
			lightsV[i]->draw();

		lightsV[i]->disable();
		if(parser.lights[i]->enabled)
			lightsV[i]->enable();
		lightsV[i]->update();
	}
	// Draw axis
	axis.draw();

	if(parser.globals->drawing.mode == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if(parser.globals->drawing.mode == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	drawGraph(parser.graph->rootID,parser.graph->nodes[parser.graph->rootID]->apperanceRef);

	glutSwapBuffers();
}

void ANFScene::drawGraph(string nodeID,string app)
{

	Node Cnode;
	Cnode = *parser.graph->nodes[nodeID];
	if(Cnode.apperanceRef == "" || Cnode.apperanceRef == "inherit")
		Cnode.apperanceRef = app;

	if(Cnode.apperanceRef != "")
		parser.appearances[Cnode.apperanceRef]->appCGF->apply();

	glMultMatrixf(Cnode.matrix);

	for(int i = 0; i < Cnode.primitives.size(); i++)
	{
		if(Cnode.apperanceRef != "inherit")
		{
			if(parser.appearances[Cnode.apperanceRef]->isTexApp)
				(*Cnode.primitives[i]).draw(parser.textures[parser.appearances[Cnode.apperanceRef]->textureRef]);
			else
				(*Cnode.primitives[i]).draw();
		}
		else
			(*Cnode.primitives[i]).draw();
	}

	for(int i = 0; i < Cnode.descendants.size(); i++)
	{
		glPushMatrix();
		drawGraph(Cnode.descendants[i],Cnode.apperanceRef);
		glPopMatrix();
	}


}



