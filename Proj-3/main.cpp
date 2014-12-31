#include <iostream>
#include <exception>



#include "CGFapplication.h"
//#include "ANFScene.h"
//#include "Interface.h"
#include "PickScene.h"
#include "PickInterface.h"
#include "socket.h"
using std::cout;
using std::exception;



int main(int argc, char* argv[]){
	
	socketConnect();
	string x,y;
	
	

	cout<<"Lines: ";cin>>x;
	x.append(".\n");
	char *a=new char[x.size()+1];
	a[x.size()]=0;
	memcpy(a,x.c_str(),x.size());
	envia(a, strlen(a));
	char ans[128],xxx[128];
	recebe(ans);

	cout<<"Columns: ";cin>>y;
	y.append(".\n");
	char *b=new char[y.size()+1];
	b[y.size()]=0;
	memcpy(b,y.c_str(),y.size());
	envia(b, strlen(b));
	recebe(xxx);

	quit();
	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		//app.setScene(new ANFScene("LAIG_TP1_ANF_T4_G8_v2 (3).anf"));
		//app.setInterface(new Interface());
		app.setScene(new PickScene());
		app.setInterface(new PickInterface());
		app.run();
	}
	catch(GLexception& ex) {
		cout << "Erro: " << ex.what();
		getchar();
		return -1;
	}
	catch(exception& ex) {
		cout << "Erro inesperado: " << ex.what();
		getchar();
		return -1;
	}
	
	quit();
	return 0;
}