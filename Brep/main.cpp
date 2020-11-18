// Brep.cpp: 定义应用程序的入口点。
//
#include"src/sweeper/Sweeper.h"
#include<iostream>
using namespace std;
int main(int argc, char** argv)
{
	std::cout << "Hello Sth." << std::endl;
	std::cout << "1. set scene." << std::endl;
	Scene* scene = new Scene();
	Sweeper* sweeper = Sweeper::getInstance();

	sweeper->setScene(scene);
	std::cout << "2. build scene." << std::endl;
	sweeper->buildScene("D:\\VisualStudioWorkspace\\Brep\\Brep\\inputFile\\data.json");
	
	std::cout << "3. begin sweep." << std::endl;
	sweeper->sweep();
	std::cout << "3.5 show information." << std::endl;
	sweeper->showInfo();

	std::cout << "4. draw the scene." << std::endl;
	sweeper->draw();
	
	return 0;
}
