#include"datastruct/Face.h"
#include<iostream>
Face* Face::getPre()const {
	return this->pre_;
}
Face* Face::getNext()const {
	return this->next_;
}
Loop* Face::getOutLoop()const {
	return this->out_loop_;
}
Loop* Face::getInLoop() const
{
	return this->in_loop_;
}
Solid* Face::getSolid()const {
	return this->solid_;
}
glm::vec3 Face::getColor() const{
	return this->color_;
}

glm::vec3 Face::getNormal() const
{
	// 两个向量的叉乘
	glm::vec3 vector_1;
	glm::vec3 vector_2;
	glm::vec3 normal = glm::vec3(-1);
	Halfedge* temp_halfedge = this->getOutLoop()->getHalfedge();
	// 得到三个点
	glm::vec3 vertex_position_1 = temp_halfedge->getStartVertex()->getPosition();
	temp_halfedge = temp_halfedge->getNext();
	glm::vec3 vertex_position_2 = temp_halfedge->getStartVertex()->getPosition();
	temp_halfedge = temp_halfedge->getNext();
	glm::vec3 vertex_position_3 = temp_halfedge->getStartVertex()->getPosition();
	
	vector_1 = vertex_position_2 - vertex_position_1;
	vector_2 = vertex_position_3 - vertex_position_2;

	normal = glm::normalize(glm::cross(vector_1, vector_2));
		
	return normal;
}

void Face::setPre(Face* preFace) {
	this->pre_ = preFace;
}
void Face::setNext(Face* nextFace) {
	this->next_ = nextFace;
}
void Face::setOutLoop(Loop* outLoop) {
	this->out_loop_ = outLoop;
}

void Face::setSolid(Solid* solid) {
	this->solid_ = solid;
}
void Face::setColor(glm::vec3 color) {
	this->color_ = color;
}
void Face::setColor(float r, float g, float b) {
	this->color_.r = r;
	this->color_.g = g;
	this->color_.b = b;
}

void Face::showInfo()
{
	glm::vec3 normal = this->getNormal();
	std::cout << "id:" << this->id_ << " normal:" << "(" << normal.x << "," << normal.y << "," << normal.z << ")" << std::endl;

	std::cout << "		out loop:";
	Loop* out = this->out_loop_;
	Halfedge* temp_halfedge = this->out_loop_->getHalfedge();
	
	while (temp_halfedge->getNext() != this->out_loop_->getHalfedge()) {
		glm::vec3 position = temp_halfedge->getStartVertex()->getPosition();
		std::cout << "(" << position.x << "," << position.y << "," << position.z << ") -> ";
		temp_halfedge = temp_halfedge->getNext();
	}
	glm::vec3 position = temp_halfedge->getStartVertex()->getPosition();
	std::cout << "(" << position.x << "," << position.y << "," << position.z << ")" << std::endl;;
	
	
	std::cout << "		in loop:" << std::endl;
	Loop* in = this->in_loop_;
	if (in==nullptr) {
		std::cout << "		no in loop." << std::endl;
		return;
	}
	int num = 0;
	while (in!=nullptr) {
		temp_halfedge = in->getHalfedge();
		std::cout << "		" << num++ << ":";
		while (temp_halfedge->getNext() != in->getHalfedge())
		{
			position = temp_halfedge->getStartVertex()->getPosition();
			std::cout << "(" << position.x << "," << position.y << "," << position.z << ") -> ";
			temp_halfedge = temp_halfedge->getNext();
		}
		position = temp_halfedge->getStartVertex()->getPosition();
		std::cout << "(" << position.x << "," << position.y << "," << position.z << ")" << std::endl;
		
		in = in->getNext();
	}
}

void Face::addInLoop(Loop* in_loop)
{
	if (this->in_loop_ == nullptr) {
		this->in_loop_ = in_loop;
	}
	else {
		Loop* temp_loop = this->in_loop_;
		while (temp_loop->getNext()!=nullptr) {
			temp_loop = temp_loop->getNext();
		}
		temp_loop->setNext(in_loop);
		in_loop->setPre(temp_loop);
	}
}
