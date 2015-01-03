#ifndef _GAME_H_
#define _GAME_H_

#include "Pieces.h"
#include "TextObject.h"
class LightG
{
public:
	string id;
	bool enabled;
	CGFlight *light;
};

class Move
{
public:
	char direction;
	float oldP[2];
	float newP[2];
	bool disk;
	int number;
};

class Game
{
public:
	Board brd;
	int activePl;
	vector<Camera*> cams;
	vector<LightG*> lights;
	vector<Move*> listPlays;
	vector<Move> tempPlays;
	int activeCam;
	TextObject* plText;
	int size;
	Game(){}
	Game(int size){
		this->size = size;
		brd = Board(size,"textures/Border.jpg","textures/Border.jpg");
		activePl = 0;
		activeCam = 2;

		COrtho *c1 = new COrtho;
		c1->id = "Top view";
		c1->direction = "y";
		c1->near = -60;
		c1->far = 70;
		c1->left = 0;
		c1->right = 15;
		c1-> top = 5;
		c1->bottom = -15;

		cams.push_back(c1);

		CPerspective *c2 = new CPerspective();
		c2->id = "Perspective view";
		c2->near = 0;
		c2->far = 70;
		c2->angle = 140;

		c2->pos[0] = 20;
		c2->pos[1] = 10;
		c2->pos[2] = 20;

		c2->target[0] = 0;
		c2->target[1] = 0;
		c2->target[2] = 0;

		cams.push_back(c2);

		float pos[4] = {2.5,7,2.5,1};

		LightG *light0 = new LightG();
		light0->light = new CGFlight(GL_LIGHT0, pos);
		light0->enabled = true;
		light0->id = "Main Light";

		LightG *light1 = new LightG();
		light1->light = new CGFlight(GL_LIGHT1, pos);
		light1->enabled = false;
		light1->id = "Light Sel";

		lights.push_back(light0);
		lights.push_back(light1);

		plText = new TextObject();
	}

	bool calculateMove(float oldP[2], float newP[2], bool disk){
		if(oldP[0] != newP[0] && oldP[1] != newP[1])
			return false;
		else{
			if(tempPlays.size() == 0){
				//validar jogada em prolog
				//se for valido
				Move m;
				m.oldP[0] = oldP[0];
				m.oldP[1] = oldP[1];

				m.newP[0] = newP[0];
				m.newP[1] = newP[1];
				m.disk = disk;

				tempPlays.push_back(m);


				brd.plIndex[activePl][0] = newP[0];
				brd.plIndex[activePl][1] = newP[1];

				if(disk){
					brd.coords[oldP[0]][oldP[1]][1]--;
					brd.coords[newP[0]][newP[1]][1]++;
					brd.dsk[oldP[0]*size+oldP[1]]->move( brd.coords[newP[0]][newP[1]][0],brd.coords[newP[0]][newP[1]][1],brd.coords[newP[0]][newP[1]][2]);

					if(activePl)
						brd.pl2.move(brd.coords[newP[0]][newP[1]][0],(brd.coords[newP[0]][newP[1]][1]+1)*0.5,brd.coords[newP[0]][newP[1]][2]);
					else
						brd.pl1.move(brd.coords[newP[0]][newP[1]][0],(brd.coords[newP[0]][newP[1]][1]+1)*0.5,brd.coords[newP[0]][newP[1]][2]);

				}
				else
					if(activePl)
						brd.pl2.move(brd.coords[newP[0]][newP[1]][0],brd.coords[newP[0]][newP[1]][1],brd.coords[newP[0]][newP[1]][2]);
					else
						brd.pl1.move(brd.coords[newP[0]][newP[1]][0],brd.coords[newP[0]][newP[1]][1],brd.coords[newP[0]][newP[1]][2]);

				return true;
			}
		}
	}
	bool validMove(){
		if(tempPlays.size()> 0){
			Move *m = new Move();
			m = &tempPlays[tempPlays.size()-1];
			listPlays.push_back(m);
			tempPlays.clear();
			return true;
		}
		else{
			printf("You must move your piece before ending turn\n\n");
			return false;
		}
	}

	bool undo(){
		if(tempPlays.size() > 0)
		{
			Move m = tempPlays[tempPlays.size()-1];

			if(m.disk){
				brd.coords[m.newP[0]][m.newP[1]][1]--;
				brd.coords[m.oldP[0]][m.oldP[1]][1]++;
				brd.dsk[m.oldP[0]*size+m.oldP[1]]->move( brd.coords[m.oldP[0]][m.oldP[1]][0],brd.coords[m.oldP[0]][m.oldP[1]][1],brd.coords[m.oldP[0]][m.oldP[1]][2]);
			}

			brd.plIndex[activePl][0] = m.oldP[0];
			brd.plIndex[activePl][1] = m.oldP[1];

			if(activePl)
				brd.pl2.move(brd.coords[m.oldP[0]][m.oldP[1]][0],brd.coords[m.oldP[0]][m.oldP[1]][1],brd.coords[m.oldP[0]][m.oldP[1]][2]);
			else
				brd.pl1.move(brd.coords[m.oldP[0]][m.oldP[1]][0],brd.coords[m.oldP[0]][m.oldP[1]][1],brd.coords[m.oldP[0]][m.oldP[1]][2]);

			tempPlays.pop_back();
			return true;
		}
		else
			return false;

	}

	void update(unsigned long t){
		if(activePl)
			brd.pl2.update(t);
		else
			brd.pl1.update(t);

		for(unsigned int i = 0; i < brd.dsk.size();i++){
			brd.dsk[i]->update(t);
		}
	}

};


#endif
