#include "Scene.h"
#include <iostream>
#include<string>
Solid* Scene::getSolid()
{
	return this->now_solid_;
}

/*
* parameter: 新点的坐标
* return: 新点
*/
Vertex* Scene::eulerOperateMVFS(glm::vec3 position)
{
	std:: cout<< "Begin MVFS" << std::endl;
	// new
	Solid* solid = new Solid(solid_cnt++, nullptr, nullptr, nullptr, nullptr);
	Vertex* vertex = new Vertex(vertex_cnt++, nullptr, nullptr, position);
	Face* face = new Face(face_cnt++, solid, nullptr, nullptr, nullptr);
	Loop* loop = new Loop(loop_cnt++, face, nullptr, nullptr, nullptr);
	
	// relate 
	this->now_solid_ = solid;

	solid->addFace(face);
	face->setOutLoop(loop);
	//face->setLoop(loop);
	loop->setHalfedge(nullptr);

	std:: cout<< "End MVFS" << std::endl;
	return vertex;

}

/*
* parameter: 第一个vertex，新边所在的loop，新vertex的坐标
* return: 新edge
*/
Edge* Scene::eulerOperateMEV(Vertex* vertex_1, Loop* loop, glm::vec3 position)
{
	std::cout << "Begin MEV" << std::endl;
	// new
	Edge* edge = new Edge(edge_cnt++, this->now_solid_,nullptr, nullptr, nullptr, nullptr);
	
	Vertex* vertex_2 = new Vertex(vertex_cnt++, nullptr, nullptr, position);
	Halfedge* halfedge_1 = new Halfedge(halfedge_cnt++, loop, nullptr, nullptr, edge, vertex_1, nullptr);
	Halfedge* halfedge_2 = new Halfedge(halfedge_cnt++, loop, halfedge_1, nullptr, edge, vertex_2, halfedge_1);

	// relate
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
 

	// update 
	std::cout << "End MEV" << std::endl;
	return edge;
}


/*
* parameter: 第一个vertex，第二个vertex，第一个和第二个vertex原来所在的loop
* return: 接下来需要操作的loop，这个loop与halfedge_2相关联，原loop与halfedge_1相关联。
* 在新建普通的边和建内环时的边时，输入参数vertex_1和vertex_2在实体中的顺序相反。
*/
Loop* Scene::eulerOperateMEF(Vertex* vertex_1, Vertex* vertex_2, Loop* pre_loop)
{
	
	std::cout << "Bgein MEF" << std::endl;
	if (pre_loop==nullptr) {
		std::cerr << "ERROR: Pre_loop is nullptr, in Scene.cpp_eulerOperateMEF()." << std::endl;
		return nullptr;
	}

	// new 
	Face* face = new Face(face_cnt++, now_solid_, nullptr, nullptr, nullptr);
	Loop* loop = new Loop(loop_cnt++, face, nullptr, nullptr, nullptr);

	Edge* edge = new Edge(edge_cnt++, now_solid_, nullptr, nullptr, nullptr, nullptr);
	Halfedge* halfedge_1 = new Halfedge(halfedge_cnt++, loop, nullptr, nullptr, edge, vertex_1, nullptr);
	Halfedge* halfedge_2 = new Halfedge(halfedge_cnt++, loop, nullptr, nullptr, edge, vertex_2, halfedge_1);
 
	// relate
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
		return nullptr;
	}
	//glm::vec3 temp_vertex_position = temp_halfedge_1->getStartVertex()->getPosition();
	//std::cout << "vertex id::" << temp_halfedge_1->getStartVertex()->getId();
	//std::cout << "temp_halfedge_1->startVertex->getPosition is:(" << temp_vertex_position.x << "," << temp_vertex_position.y << "," << temp_vertex_position.z << ")" << std::endl;
	//
	temp_halfedge_2 = pre_loop->getHalfedge();
	while (temp_halfedge_2 != nullptr && temp_halfedge_2->getNext()->getStartVertex() != vertex_1)
	{
		temp_halfedge_2 = temp_halfedge_2->getNext();
	}

	if (temp_halfedge_2 == nullptr) {
		std::cerr << "ERROR: temp_halfedge_2 is nullptr in Scene.cpp_eulerOperateMEF()." << std::endl;
		return nullptr;
	}

	//temp_vertex_position = temp_halfedge_2->getStartVertex()->getPosition();
	/*std::cout << "vertex id::" << temp_halfedge_2->getStartVertex()->getId();
	std::cout << "temp_halfedge_2->startVertex->getPosition is:(" << temp_vertex_position.x << "," << temp_vertex_position.y << "," << temp_vertex_position.z << ")" << std::endl;*/

	halfedge_1->setNext(temp_halfedge_1->getNext());
	halfedge_1->getNext()->setPre(halfedge_1);
	halfedge_1->setPre(temp_halfedge_2);
	
	halfedge_2->setNext(temp_halfedge_2->getNext());
	temp_halfedge_2->setNext(halfedge_1);
	halfedge_2->getNext()->setPre(halfedge_2);
	halfedge_2->setPre(temp_halfedge_1);
	temp_halfedge_1->setNext(halfedge_2);

	// update loop and halfedge_1_2

	halfedge_1->setLoop(pre_loop);
	pre_loop->setHalfedge(halfedge_1);
	Halfedge* temp_halfedge = halfedge_1->getNext();
	//std::cout << "pre_loop vertexes:";

	while (temp_halfedge!=nullptr && temp_halfedge!=halfedge_1) {
		temp_halfedge->setLoop(pre_loop);
		//temp_vertex_position = temp_halfedge->getStartVertex()->getPosition();
		//std::cout << "(" << temp_vertex_position.x << "," << temp_vertex_position.y << "," << temp_vertex_position.z << ")" << "->";
		temp_halfedge = temp_halfedge->getNext();
	}
	if (temp_halfedge==nullptr) {
		std::cout << "ERROR: temp_halfedge is nullptr in Scene.cpp_MEF()." << std::endl;
		return nullptr;
	}
	//temp_vertex_position = temp_halfedge->getStartVertex()->getPosition();
	//std::cout << "(" << temp_vertex_position.x << "," << temp_vertex_position.y << "," << temp_vertex_position.z << ")" << "->";
	//std::cout<<std::endl;

	//std::cout << "new_loop vertexes:";
	halfedge_2->setLoop(loop);
	loop->setHalfedge(halfedge_2);
	temp_halfedge = halfedge_2->getNext();

	while (temp_halfedge!=nullptr && temp_halfedge!=halfedge_2) {
		temp_halfedge->setLoop(loop);
		//temp_vertex_position = temp_halfedge->getStartVertex()->getPosition();
		//std::cout << "(" << temp_vertex_position.x << "," << temp_vertex_position.y << "," << temp_vertex_position.z << ")" << "->";
		temp_halfedge = temp_halfedge->getNext();
	}
	//temp_vertex_position = temp_halfedge->getStartVertex()->getPosition();
	//std::cout << "(" << temp_vertex_position.x << "," << temp_vertex_position.y << "," << temp_vertex_position.z << ")" << "->";

	// update VFESHR
	std::cout << "End MEF" << std::endl;
	return loop;
}
/*
* parameter: 需要被删除的edge
* return: 新生成的loop
*/
Loop* Scene::eulerOperateKEMR(Edge* edge)
{
	std::cout << "Begin KEMR" << std::endl;
	
	Halfedge* halfedge_1 = edge->getFirstHalfedge();
	Halfedge* halfedge_2 = edge->getSecondHalfedge();
	if (halfedge_1 == nullptr || halfedge_2 == nullptr) {
		std::cerr << "ERROR: halfedge_1 or halfedge_2 is nullptr in Scene.cpp_KEMR()." << std::endl;
		return nullptr;
	}
	Vertex* vertex_1 = halfedge_1->getStartVertex();
	Vertex* vertex_2 = halfedge_2->getStartVertex();
	Face* face = halfedge_1->getLoop()->getFace();
	Loop* out_loop = face->getOutLoop();

	// new 
	Loop* in_loop = new Loop(loop_cnt++, face, nullptr, nullptr, halfedge_1->getNext());
	out_loop->setHalfedge(halfedge_2->getNext());
	
	// relate
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
	
	std::cout << "End KEMR" << std::endl;
	//delete edge;
	return in_loop;
}

/*
* parameter: 需要被删除的face，删除face后产生的loop所在的face
* return: 产生发loop
*/
Loop* Scene::eulerOperateKFMRH(Face* face_1, Face* face_2)
{	
	std::cout << "Begin KFMRH" << std::endl;
	// new
	Loop* in_loop = new Loop(loop_cnt++, face_2, nullptr, nullptr, face_1->getOutLoop()->getHalfedge());

	face_2->addInLoop(in_loop);
	//
	now_solid_->deleteFace(face_1);
	std::cout << "End KFMRH" << std::endl;

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
