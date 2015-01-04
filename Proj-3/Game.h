#ifndef _GAME_H_
#define _GAME_H_

#include "Pieces.h"
#include "TextObject.h"
#include "socket.h"
#include <sstream>
#include <iostream>

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
	float oldP[2];
	float newP[2];
	float enemP[2];
	bool disk;
	int number,stack;	
	string direction;
	int points;
	string brd;
};

class Game
{
public:
	Board brd;
	Socket *as;
	char mensagem[255];
	int activePl;
	vector<int> playerInfo; // 0 is active player, 1 is points of player 1 and 2 is points of player 2
	vector<Camera*> cams;
	vector<LightG*> lights;
	vector<Move*> listPlays;
	vector<Move*> tempPlays;
	int tempoMax;
	unsigned long t0;
	bool inicia,nextTurn;
	int activeCam;
	TextObject* plText;
	int size,difficulty,versus;

	float wA, inc,startAng;
	unsigned long t0R;

	Game(){}
	Game(int size,int tempoMax){
		as=new Socket();
		as->socketConnect();
		this->size = size;
		this->tempoMax = tempoMax*1000;
		inicia = true;
		brd = Board(size,"textures/Border.jpg","textures/Border.jpg");
		activePl = 0;
		t0 = 0;


		nextTurn = false;
		wA = 180;
		t0R = 0;
		inc = 0;
		startAng = 0;

		playerInfo.push_back(0);
		playerInfo.push_back(0);
		playerInfo.push_back(0);
		playerInfo.push_back(0);
		playerInfo.push_back(0);
		playerInfo.push_back(0);
		versus = 0;
		difficulty = 0;
		activeCam = 2;

		COrtho *c1 = new COrtho;
		c1->id = "Top view";
		c1->direction = "y";
		c1->near1 = -60;
		c1->far1 = 70;
		c1->left = 0;
		c1->right = 15;
		c1->top = 5;
		c1->bottom = -15;

		cams.push_back(c1);

		CPerspective *c2 = new CPerspective();
		c2->id = "Perspective view";
		c2->near1 = 0;
		c2->far1 = 70;
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

				//se for valido
				Move *m=new Move();
				m->oldP[0] = oldP[0];
				m->oldP[1] = oldP[1];

				m->newP[0] = newP[0];
				m->newP[1] = newP[1];
				m->disk = disk;
				m->points = playerInfo[activePl+1];
				
				m=constructBoard(m);
				cout<<m->points;
				//validateMoveU(Line,Column,NewLine,NewColumn,/ LineE,ColumnE,Dir,Nmb,Dsk,Brd,/Symbol/,Points,PrevChoice)
				string send="validate.\n";memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255);

				stringstream oo,o1,o2,o3,o4,o5,o6,o7,o8; 
				oo<<m->oldP[0];send=oo.str();send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				o1<<m->oldP[1];send=o1.str();send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				o2<<m->newP[0];send=o2.str();send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				o3<<m->newP[1];send=o3.str();send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				o4<<m->enemP[0];send=o4.str();send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				o5<<m->enemP[1];send=o5.str();send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				send=m->direction;send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				o7<<m->number;send=o7.str();send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				
				if(m->disk==true)
					send="y.\n";
				else
					send="n.\n";
				memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();

				send=m->brd;send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255);
				
				if(playerInfo[0]==0)
					send="[36].\n";
				else
					send="[35].\n";
				memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255);
				o8<<m->points;send=o8.str();send.append(".\n");memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				send="n.\n";memcpy(mensagem,send.c_str(),send.size());as->envia(mensagem,send.size());memset(mensagem,0,255); oo.clear();
				
				memset(mensagem,0,255);
				as->recebe(mensagem);
				if(mensagem=="no\n"){

				}
				else{
					memset(mensagem,0,255);
					as->recebe(mensagem);
					//neste ponto mensagem=pontos
				}
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
	void validMove(){
		if(tempPlays.size()> 0){
			Move *m = new Move();
			m = tempPlays[tempPlays.size()-1];
			listPlays.push_back(m);
			tempPlays.clear();
			brd.activePl = activePl;
		}
		else{
			Move *m = NULL;
			listPlays.push_back(m);
			tempPlays.clear();
			brd.activePl = activePl;
		}
	}

	bool undo(){
		if(tempPlays.size() > 0)
		{
			Move *m = tempPlays[tempPlays.size()-1];

			if(m->disk){
				brd.coords[m->newP[0]][m->newP[1]][1]--;
				brd.coords[m->oldP[0]][m->oldP[1]][1]++;
				brd.dsk[m->oldP[0]*size+m->oldP[1]]->move( brd.coords[m->oldP[0]][m->oldP[1]][0],brd.coords[m->oldP[0]][m->oldP[1]][1],brd.coords[m->oldP[0]][m->oldP[1]][2]);
			}

			brd.plIndex[activePl][0] = m->oldP[0];
			brd.plIndex[activePl][1] = m->oldP[1];

			if(activePl)
				brd.pl2.move(brd.coords[m->oldP[0]][m->oldP[1]][0],brd.coords[m->oldP[0]][m->oldP[1]][1],brd.coords[m->oldP[0]][m->oldP[1]][2]);
			else
				brd.pl1.move(brd.coords[m->oldP[0]][m->oldP[1]][0],brd.coords[m->oldP[0]][m->oldP[1]][1],brd.coords[m->oldP[0]][m->oldP[1]][2]);

			tempPlays.pop_back();
			return true;
		}
		else
			return false;

	}

	void update(unsigned long t){


		//counter for each play
		if(inicia){ // inicio de uma nova jogada
			t0 = t;
			inicia = false;
		}
		unsigned long dt = t - t0;
		if (dt >= tempoMax) {
			printf("Acabou o tempo, proximo jogador\n\n");

			validMove();
			nextTurn = true;
			t0R = 0;

			if(activePl)
				activePl = 0;
			else
				activePl = 1;

			playerInfo[0] = activePl;
			inicia = true;
		}
		else{
			playerInfo[3] = dt/1000;
			playerInfo[4] = (dt / 100) % 10;
			playerInfo[5] = (dt / 10) % 10;
			printf("%d\n",dt);
		}


		if(activePl)
			brd.pl2.update(t);
		else
			brd.pl1.update(t);

		for(unsigned int i = 0; i < brd.dsk.size();i++){
			brd.dsk[i]->update(t);
		}

		//Switch point of view
		if (!nextTurn)
			return;
		if (t0R == 0)
			t0R = t;
		unsigned long dtR = t - t0R;
		if (dtR < 500) {
			inc = (dtR / 500.0) * wA;
		} else {
			inicia = true;
			nextTurn = false;
			startAng += 180;
			inc = 0;
		}
	}

	Move* constructBoard(Move *n){
		Move *m=new Move();
		m=n;
		float nPos[2];
		float pos[2];
		pos[0] = m->oldP[0];
		pos[1] = m->oldP[1];
		char nonC,actC;
		int non;
		if(activePl){
			non = 0;
			nonC = 'X';
			actC = 'Y';
		}
		else{ 
			non = 1;
			nonC = 'Y';
			actC = 'X';
		}

		if((m->oldP[0] == m->newP[0]) && (m->newP[1] > m->oldP[1])){
			m->direction = "r";
			m->number = m->newP[1] - m->oldP[1];
		}
		else if((m->oldP[0] == m->newP[0]) && (m->newP[1] < m->oldP[1])){
			m->direction = "l";
			m->number =  m->oldP[1] - m->newP[1];
		}
		else if((m->oldP[1] == m->newP[1]) && (m->newP[0] < m->oldP[0])){
			m->direction = "u";
			m->number = m->newP[0] - m->oldP[0];
		}
		else if((m->oldP[1] == m->newP[1]) && (m->newP[0] > m->oldP[0])){
			m->direction = "d";
			m->number =  m->oldP[0] - m->newP[0];
		}

		nPos[0] = brd.plIndex[non][0];
		nPos[1] = brd.plIndex[non][1];

		m->enemP[0] = nPos[0];
		m->enemP[1] = nPos[1];

		string str = "[";
		for(int i = 0; i < size;i++){
			str += "[";
			for(int j = 0; j < size;j++){
				ostringstream os;
				if(i == pos[0] && j == pos[1]){
					m->stack = brd.coords[i][j][1];
					os << "[" << actC << "|" << brd.coords[i][j][1] << "]";
				}
				else if(i == nPos[0] && j == nPos[1]){
					os << "[" << nonC << "|" << brd.coords[i][j][1] << "]";
				}
				else
					os << brd.coords[i][j][1];

				str += os.str();
				if(j != size-1)
					str += ",";
			}
			str += "]";
			if(i != size-1)
				str += ",";
		}
		str += "]";
		m->brd = str;
		return m;
	}



};


#endif
