#include "Parser.h"


Parser::Parser()
{
	globals = new Globals();
	graph = new Graph();

}


Parser::~Parser()
{
	cameras.clear();
	lights.clear();
	textures.clear();
	appearances.clear();
}
