#include <iostream>
#include <exception>

#include "CGFapplication.h"
#include "PickInterface.h"
#include "PickScene.h"
#include "socket.h"
using std::cout;
using std::exception;



int main(int argc, char* argv[]){
	
	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

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
	
	return 0;
}