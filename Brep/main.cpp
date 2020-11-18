// Brep.cpp: 定义应用程序的入口点。
//
#include"src/sweeper/Sweeper.h"

#include<iostream>

using namespace std;
int main(int argc, char** argv)
{
	std::cout << "Hello Sth." << std::endl;
	Scene* scene = new Scene();
	Sweeper* sweeper = Sweeper::getInstance();

	sweeper->setScene(scene);

	sweeper->buildScene("D:\\VisualStudioWorkspace\\Brep\\Brep\\inputFile\\cad.json");
	
	sweeper->sweep();

	sweeper->showInfo();
	
	sweeper->draw();
	
	return 0;
}
