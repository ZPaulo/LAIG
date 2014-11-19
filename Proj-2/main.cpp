
#include <iostream>
#include "ANFScene.h"
#include "CGFapplication.h"
#include "Interface.h"

using namespace std;

int main(int argc, char* argv[]){

	char a;
	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		app.setScene(new ANFScene("LAIG_TP1_ANF_T4_G8_v2.anf"));
		app.setInterface(new Interface());
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
	return 0;
}