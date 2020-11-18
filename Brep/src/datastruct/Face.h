#pragma once
#include<glm/glm.hpp>
#include"datastruct/Solid.h"
#include"datastruct/Loop.h"
#include"datastruct/AllDataStruct.h"
#include<vector>
class Face
{
public:
	Face(int id, Solid* solid, Face* pre, Face* next, Loop* out_loop) {
		this->id_ = id;
		this->solid_ = solid;
		this->pre_ = pre;
		this->next_ = next;
		this->out_loop_ = out_loop;
		this->in_loop_ = nullptr;
		this->color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	};
	~Face() {
		;
	};

	//// get and set
	int getId() {
		return this->id_;
	};
	Face* getPre()const;
	Face* getNext()const;
	Loop* getOutLoop()const;
	
	Loop* getInLoop()const;

	/*std::vector<Loop*> getInLoop() const;*/
	
	Solid* getSolid()const;
	glm::vec3 getColor()const;
	//int getOutLoopCount()const;
	//int getInLoopCount()const;
	// 得到面的法向
	glm::vec3 getNormal()const;
	void setPre(Face* preFace);
	void setNext(Face* nextFace);

	void setOutLoop(Loop* outLoop);
	void addInLoop(Loop* inLoop);

	void setSolid(Solid* solid);
	void setColor(glm::vec3 color);
	void setColor(float r, float g, float b);

	void showInfo();

private:

	int id_;
	Face* pre_;
	Face* next_;
	
	Loop* out_loop_;
	Loop* in_loop_;

	//std::vector<Loop* > in_loop_;

	Solid* solid_;
	glm::vec3 color_;
};