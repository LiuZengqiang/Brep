#pragma once
#include"datastruct/Solid.h"
#include"datastruct/Face.h"
#include"datastruct/Loop.h"
#include"datastruct/Edge.h"
#include"datastruct/Halfedge.h"
#include"datastruct/Vertex.h"
#include"scene/Scene.h"
#define NDEBUG
#include"freeglut/freeglut.h"
#include<string>
#include<vector>
#include<iostream>
#include"glm/glm.hpp"

struct Outline {
	std::vector<glm::vec2> vertexes;
};

// sweeper
// 用来绘画
// 读取参数文件 -> sweep构筑Scene -> 检测欧拉公式是否满足 -> 绘制显示
// 设置为单例类
class Sweeper
{
public:
	static Sweeper* getInstance() {

		if (currentInstance_ == nullptr) {
			currentInstance_ = new Sweeper();
		}
		return currentInstance_;
	}

	
	~Sweeper() {
		free(this->lightPosition_);
		free(this->solidRotateAngle_);
		free(this->solidColor_);
	};
	void buildScene(std::string input_file_path);

	void setScene(Scene* scene);
	// new Scene
	void sweep();
	void showInfo();
	void draw();

	void setupDisplayCallback() {
		//currentInstance_ = this;
		glutDisplayFunc(Sweeper::displayCallback);
	}
	static void displayCallback() {
		currentInstance_->myDisplay();
	};
	static void vertexCallbackS(GLvoid* vertex) {
		currentInstance_->vertexCallback(vertex);
	}
	static void beginCallbackS(GLenum which){
		currentInstance_->beginCallback(which);
	}
	static void endCallbackS() {
		currentInstance_->endCallback();
	}

	static void keyBoardsCallbackS(unsigned char key, int x, int y) {
		currentInstance_->keyBoards(key, x, y);
	}

private:
	Sweeper() {
		//this->scene_ = nullptr;
		this->sweep_scale_ = 2.0f;
		this->sweep_vector_ = glm::vec3(0.0f, 0.0f, -1.0f);
		this->scene_ = nullptr;


		this->lightPosition_ = new GLfloat[4];
		this->lightPosition_[0] = 0.0;
		this->lightPosition_[1] = 0.0;
		this->lightPosition_[2] = 0.0;
		this->lightPosition_[3] = 1.0;

		this->solidRotateAngle_ = new GLfloat[2];
		this->solidRotateAngle_[0] = 0.0;
		this->solidRotateAngle_[1] = 0.0;

		this->solidColor_ = new GLfloat[3];
		this->solidColor_[0] = 1.0;
		this->solidColor_[1] = 0.0;
		this->solidColor_[2] = 0.0;
	};

	void addVertex(Vertex* vertex);
	void myDisplay();
	void CALLBACK vertexCallback(GLvoid* vertex) {
		const GLdouble* pointer;
		pointer = (GLdouble*)vertex;
		glVertex3dv(pointer);
	}

	void CALLBACK beginCallback(GLenum which) {
		glBegin(which);
	}
	void CALLBACK endCallback() {
		glEnd();
	}

	void keyBoards(unsigned char key, int x, int y) {
		// wsad控制实体旋转
		// 4628控制光源
		switch (key)
		{
		case 'w':
			this->solidRotateAngle_[1] -= 1.0;
			break;
		case 's':
			this->solidRotateAngle_[1] += 1.0;
			break;
		case 'a':
			this->solidRotateAngle_[0] -= 1.0;
			break;
		case 'd':
			this->solidRotateAngle_[0] += 1.0;
			break;
		case '6':
			this->lightPosition_[0] += 0.1;
			break;
		case '4':
			this->lightPosition_[0] -= 0.1;
			break;
		case '2':
			this->lightPosition_[2] += 0.1;
			break;
		case '8':
			this->lightPosition_[2] -= 0.1;
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}
	Vertex* getVertex(int index);

	GLfloat* lightPosition_;
	GLfloat* solidRotateAngle_;
	GLfloat* solidColor_;

	Scene* scene_;
	Outline outline_;	// 轮廓
	std::vector<Outline> in_ring_;	// 内环
	glm::vec3 sweep_vector_;	// sweep 方向向量
	float sweep_scale_;
	std::vector<Vertex* > all_vertexes;	//所有的点
	static Sweeper* currentInstance_;
};