#ifndef _GAME_H_
#define _GAME_H_

#include "Pieces.h"
#include "socket.h"
class Game
{
public:
	Board brd;
	int activePl;
	Game(){}
	
	void init(){
		Socket as;
		as.socketConnect();
		
		char mensagem[255];
		
		string num;

		//How many lines will the board have
		as.recebe(mensagem);
		num="7.\n";
		memcpy(mensagem,num.c_str(),num.size());
		as.envia(mensagem,num.size());
		
		//How many columns will the board have?
		as.recebe(mensagem);
		num="7.\n";
		memcpy(mensagem,num.c_str(),num.size());
		as.envia(mensagem,num.size());

		//Modo d3 jogo H-H Ai-H
		as.recebe(mensagem);
		num="h-h.\n";
		memcpy(mensagem,num.c_str(),num.size());
		as.envia(mensagem,strlen(mensagem));

		//Random or Smart
		as.recebe(mensagem);
		num="Smart.\n";
		memcpy(mensagem,num.c_str(),num.size());
		as.envia(mensagem,strlen(mensagem));
		
		cout<<"ooooo\n";
		brd=Board(7,"textures/Border.jpg","textures/Border.jpg");
		activePl=0;
	


		//get board
		char board[255];
		as.recebe(board);

		//'Press enter to start'
		string x="\n";
		memcpy(board,x.c_str(),x.size());
		as.envia(board,strlen(board));
		
	
	}
};


#endif
