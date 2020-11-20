#include "Scene.h"
#include <iostream>
#include<string>
Solid* Scene::getSolid()
{
	return this->now_solid_;
}

Vertex* Scene::eulerOperateMVFS(glm::vec3 position)
{
	std::cout << " MVFS " << "->";

	Solid* solid = new Solid(solid_cnt++, nullptr, nullptr, nullptr, nullptr);
	Vertex* vertex = new Vertex(vertex_cnt++, nullptr, nullptr, position);
	Face* face = new Face(face_cnt++, solid, nullptr, nullptr, nullptr);
	Loop* loop = new Loop(loop_cnt++, face, nullptr, nullptr, nullptr);
	
	this->now_solid_ = solid;

	solid->addFace(face);
	face->setOutLoop(loop);

	loop->setHalfedge(nullptr);

	return vertex;

}

Edge* Scene::eulerOperateMEV(Vertex* vertex_1, Loop* loop, glm::vec3 position)
{
	std::cout << " MEV " <<"->";

	Edge* edge = new Edge(edge_cnt++, this->now_solid_,nullptr, nullptr, nullptr, nullptr);
	
	Vertex* vertex_2 = new Vertex(vertex_cnt++, nullptr, nullptr, position);
	Halfedge* halfedge_1 = new Halfedge(halfedge_cnt++, loop, nullptr, nullptr, edge, vertex_1, nullptr);
	Halfedge* halfedge_2 = new Halfedge(halfedge_cnt++, loop, halfedge_1, nullptr, edge, vertex_2, halfedge_1);

	now_solid_->addEdge(edge);
 
	edge->setFirstHalfedge(halfedge_1);
	edge->setSecondHalfedge(halfedge_2);
	halfedge_1->setNext(halfedge_2); 
	halfedge_1->setMirrorHalfedge(halfedge_2);

	if (loop->getHalfedge() == nullptr) {
		halfedge_1->setPre(halfedge_2);
		halfedge_2->setNext(halfedge_1);
		loop->setHalfedge(halfedge_1);
	}
	else {
		Halfedge* temp_halfedge = loop->getHalfedge();

		while (temp_halfedge->getNext() != nullptr && temp_halfedge->getNext()->getStartVertex() != vertex_1) {
			temp_halfedge = temp_halfedge->getNext();
		}
		if (temp_halfedge == nullptr) {
			std::cerr << "ERROR: temp_halfedge is nullptr in Scene.cpp_eulerOperateMEV()." << std::endl;
			exit(1);
			return nullptr;
		}

		halfedge_2->setNext(temp_halfedge->getNext());
		halfedge_2->getNext()->setPre(halfedge_2);

		temp_halfedge->setNext(halfedge_1);
		halfedge_1->setPre(temp_halfedge);
	}

	if (vertex_1->getNext() == nullptr) {
		vertex_1->setNext(vertex_2);
		vertex_2->setPre(vertex_1);
	}
	else {
		Vertex* temp_vertex = vertex_1;
		while (temp_vertex->getNext() != nullptr) {
			temp_vertex = temp_vertex->getNext();
		}
		temp_vertex->setNext(vertex_2);
		vertex_2->setPre(temp_vertex);
	}

	return edge;
}

Loop* Scene::eulerOperateMEF(Vertex* vertex_1, Vertex* vertex_2, Loop* pre_loop)
{
	
	std::cout << " MEF" << "->";
	if (pre_loop==nullptr) {
		std::cerr << "ERROR: Pre_loop is nullptr, in Scene.cpp_eulerOperateMEF()." << std::endl;
		exit(1);
		return nullptr;
	}

	Face* face = new Face(face_cnt++, now_solid_, nullptr, nullptr, nullptr);
	Loop* loop = new Loop(loop_cnt++, face, nullptr, nullptr, nullptr);

	Edge* edge = new Edge(edge_cnt++, now_solid_, nullptr, nullptr, nullptr, nullptr);
	Halfedge* halfedge_1 = new Halfedge(halfedge_cnt++, loop, nullptr, nullptr, edge, vertex_1, nullptr);
	Halfedge* halfedge_2 = new Halfedge(halfedge_cnt++, loop, nullptr, nullptr, edge, vertex_2, halfedge_1);

	face->setOutLoop(loop);
	this->now_solid_->addFace(face);

	edge->setFirstHalfedge(halfedge_1);
	edge->setSecondHalfedge(halfedge_2);
	this->now_solid_->addEdge(edge);
 
	halfedge_1->setMirrorHalfedge(halfedge_2);
 

	Halfedge* temp_halfedge_1 = nullptr;
	Halfedge* temp_halfedge_2 = nullptr;

	temp_halfedge_1 = pre_loop->getHalfedge();
	while (temp_halfedge_1 != nullptr && temp_halfedge_1->getNext()->getStartVertex() != vertex_2) {
		temp_halfedge_1 = temp_halfedge_1->getNext();
	}
	if (temp_halfedge_1 == nullptr) {
		std::cerr << "ERROR: temp_halfedge_1 is nullptr in Scene.cpp_eulerOperateMEF()." << std::endl;
		exit(1);
		return nullptr;
	}

	temp_halfedge_2 = pre_loop->getHalfedge();
	while (temp_halfedge_2 != nullptr && temp_halfedge_2->getNext()->getStartVertex() != vertex_1)
	{
		temp_halfedge_2 = temp_halfedge_2->getNext();
	}

	if (temp_halfedge_2 == nullptr) {
		std::cerr << "ERROR: temp_halfedge_2 is nullptr in Scene.cpp_eulerOperateMEF()." << std::endl;
		exit(1);
		return nullptr;
	}

	halfedge_1->setNext(temp_halfedge_1->getNext());
	halfedge_1->getNext()->setPre(halfedge_1);
	halfedge_1->setPre(temp_halfedge_2);
	
	halfedge_2->setNext(temp_halfedge_2->getNext());
	temp_halfedge_2->setNext(halfedge_1);
	halfedge_2->getNext()->setPre(halfedge_2);
	halfedge_2->setPre(temp_halfedge_1);
	temp_halfedge_1->setNext(halfedge_2);


	halfedge_1->setLoop(pre_loop);
	pre_loop->setHalfedge(halfedge_1);
	Halfedge* temp_halfedge = halfedge_1->getNext();

	while (temp_halfedge!=nullptr && temp_halfedge!=halfedge_1) {
		temp_halfedge->setLoop(pre_loop);
		temp_halfedge = temp_halfedge->getNext();
	}
	if (temp_halfedge==nullptr) {
		std::cout << "ERROR: temp_halfedge is nullptr in Scene.cpp_MEF()." << std::endl;
		return nullptr;
	}

	halfedge_2->setLoop(loop);
	loop->setHalfedge(halfedge_2);
	temp_halfedge = halfedge_2->getNext();

	while (temp_halfedge!=nullptr && temp_halfedge!=halfedge_2) {
		temp_halfedge->setLoop(loop);

		temp_halfedge = temp_halfedge->getNext();
	}

	return loop;
}

Loop* Scene::eulerOperateKEMR(Edge* edge)
{
	std::cout << " KEM " << "->";
	
	Halfedge* halfedge_1 = edge->getFirstHalfedge();
	Halfedge* halfedge_2 = edge->getSecondHalfedge();
	if (halfedge_1 == nullptr || halfedge_2 == nullptr) {
		std::cerr << "ERROR: halfedge_1 or halfedge_2 is nullptr in Scene.cpp_KEMR()." << std::endl;
		exit(1);
		return nullptr;
	}
	Vertex* vertex_1 = halfedge_1->getStartVertex();
	Vertex* vertex_2 = halfedge_2->getStartVertex();
	Face* face = halfedge_1->getLoop()->getFace();
	Loop* out_loop = face->getOutLoop();

	Loop* in_loop = new Loop(loop_cnt++, face, nullptr, nullptr, halfedge_1->getNext());
	out_loop->setHalfedge(halfedge_2->getNext());

	Halfedge* halfedge_1_pre = halfedge_1;
	while (halfedge_1_pre!=nullptr && halfedge_1_pre->getNext()!=halfedge_1) {
		halfedge_1_pre = halfedge_1_pre->getNext();
	}

	Halfedge* halfedge_2_pre = halfedge_2;
	while (halfedge_2_pre!=nullptr &&halfedge_2_pre->getNext()!=halfedge_2) {
		halfedge_2_pre = halfedge_2_pre->getNext();
	}

	halfedge_1_pre->setNext(halfedge_2->getNext());
	halfedge_2->getNext()->setPre(halfedge_1_pre);

	halfedge_2_pre->setNext(halfedge_1->getNext());
	halfedge_1->getNext()->setPre(halfedge_2_pre);

	face->addInLoop(in_loop);
	
	this->now_solid_->deleteEdge(edge);

	return in_loop;
}

Loop* Scene::eulerOperateKFMRH(Face* face_1, Face* face_2)
{	
	std::cout << " KFMRH " << "->";
	// new
	Loop* in_loop = new Loop(loop_cnt++, face_2, nullptr, nullptr, face_1->getOutLoop()->getHalfedge());

	face_2->addInLoop(in_loop);
	//
	now_solid_->deleteFace(face_1);

	return in_loop;
}

std::vector<glm::vec2>& Scene::getOutline()
{
	return this->outline_;
}

std::vector<glm::vec2>& Scene::getInRing(int index)
{
	if (index>=in_ring_.size()) {
		std::cerr << "ERROR: ths index of in_ring out of range, needed is [0," << in_ring_.size() << ") but get " << index << "." << std::endl;
		exit(1);
		return std::vector<glm::vec2>(1);
	}
	else {
		return in_ring_[index];
	}
}

int Scene::getInRingSize()
{
	return this->in_ring_.size();
}

void Scene::addInRing(std::vector<glm::vec2>  in_ring)
{
	this->in_ring_.push_back(in_ring);
}

glm::vec3 Scene::getSweepVector()
{
	return this->sweep_vector_;
}

void Scene::setSweepVector(glm::vec3 sweep_vector)
{
	this->sweep_vector_ = sweep_vector;
}

float Scene::getSweepScale()
{
	return this->sweep_scale_;
}

void Scene::setSweepScale(float sweep_scale)
{
	this->sweep_scale_ = sweep_scale;
}

Vertex* Scene::getVertex(int index)
{
	if (index>=all_vertexes.size()) {
		std::cerr << "ERROR: vertex index out of range, needed is [0," << all_vertexes.size() << ") but get " << index << "." << std::endl;
		exit(1);
		return nullptr;
	}
	else {
		return all_vertexes[index];
	}
}

void Scene::addVertex(Vertex* vertex)
{
	this->all_vertexes.push_back(vertex);
}

int Scene::getVertexSize()
{
	return this->all_vertexes.size();
}
