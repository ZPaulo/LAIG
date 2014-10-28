#include "Parser.h"


Parser::Parser()
{
	globals = new Globals();
	graph = new Graph();

}


Parser::~Parser()
{
	while(!cameras.empty()) 
	{
		delete cameras.back(); cameras.pop_back();
	}
	while(!lights.empty()) 
	{
		delete lights.back(); lights.pop_back();
	}
	textures.clear();
	appearances.clear();
	delete(globals);
	delete(graph);
}

Node::~Node()
{
	descendants.clear();
	primitives.clear();

}

Graph::~Graph()
{
	nodes.clear();
}
