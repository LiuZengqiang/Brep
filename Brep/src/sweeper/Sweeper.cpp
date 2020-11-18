#pragma once
// 画出来线框
#include"datastruct/Solid.h"
#include"datastruct/Face.h"
#include"datastruct/Loop.h"
#include"datastruct/Edge.h"
#include"datastruct/Halfedge.h"
#include"datastruct/Vertex.h"
#include"Sweeper.h"
#include"scene/Scene.h"
#include"json/json.h"
#define NDEBUG
#include"freeglut/freeglut.h"
#include"freeglut/glut.h"
#include<fstream>
#include<iostream>
// 需要检测点
// 是否有内环
Sweeper* Sweeper::currentInstance_ = nullptr;

void Sweeper::buildScene(std::string input_file_path)
{
	std::cout << "Begin buildScene" << std::endl;
	std::ifstream ifs;
	ifs.open(input_file_path);
	if (!ifs.is_open()) {
		std::cerr << "ERROR: open file \"" << input_file_path << "\" fail in Sweeper.cpp_readFile()." << std::endl;
		return;
	}
	// read Json
	Json::Value root;
	ifs >> root;

	Json::Value face_vertexes = root["Face"]["Vertex"];
	if (face_vertexes.isNull()) {
		std::cerr << "ERROR: input file format error in \"Face\"\"Vertex\" in Sweeper.cpp." << std::endl;
		return;
	}
	for (unsigned int i = 0; i < face_vertexes.size(); i++) {
		Json::Value temp_value = face_vertexes[i];
		glm::vec2 temp_vec2 = glm::vec2(temp_value[0].asFloat(), temp_value[1].asFloat());
		this->outline_.vertexes.push_back(temp_vec2);
	}
	 
	Json::Value face_ring = root["Face"]["Ring"];
	if (face_ring.isNull()) {
		std::cout << "face_sing is null." << std::endl;
	}

	//std::cout << "face_ring.size() is:" << face_ring.size() << std::endl;
	
	for (unsigned int i = 0; i < face_ring.size(); i++) {
		Outline temp_in_ring_outline;

		for (unsigned int j = 0; j < face_ring[i].size(); j++) {
			Json::Value temp_value = face_ring[i][j];
			glm::vec2 temp_vec2 = glm::vec2(temp_value[0].asFloat(), temp_value[1].asFloat());
			temp_in_ring_outline.vertexes.push_back(temp_vec2);
		}
		this->in_ring_.push_back(temp_in_ring_outline);
	}

	// Sweepvector
	Json::Value sweep_vector = root["SweepVector"];
	if (sweep_vector.isNull()) {
		std::cout << "WARNING: input file format warning, \"SweepVector\" is null, using defalt value." << std::endl;
	}
	else {
		this->sweep_vector_.x = sweep_vector[0].asFloat();
		this->sweep_vector_.y = sweep_vector[1].asFloat();
		this->sweep_vector_.z = sweep_vector[2].asFloat();
	}

	Json::Value sweep_scale = root["SweepScale"];
	if (sweep_scale.isNull()) {
		std::cout<<"WARNING: input file format warning, \"SweepScale\" is null, using defalt value." << std::endl;
	}
	else {
		this->sweep_scale_ = sweep_scale.asFloat();
		//std::cout << "sweep_scale(sweep_scale_) is:" << std::endl;
		//std::cout << sweep_scale_ << std::endl;
	}

	Json::Value solid_color = root["Color"];
	if (solid_color.isNull()) {
		std::cout << "WARNING: input file format warning, \"Color \" is null, using defalt value. " << std::endl;
	}
	else {
		this->solidColor_[0] = solid_color[0].asFloat();
		this->solidColor_[1] = solid_color[1].asFloat();
		this->solidColor_[2] = solid_color[2].asFloat();
	}

	std::cout << "End buildScene" << std::endl;
	return;
}

void Sweeper::setScene(Scene* scene)
{
	this->scene_ = scene;
}

void Sweeper::sweep()
{
	std::cout << "Begin Sweep" << std::endl;
	if (this->scene_ == nullptr) {
		std::cout << "WARRING: Please call initial function first." << std::endl;
		return;
	}
	
	// sweep
	// 构造需要sweep的面， 并且sweep
	// 经过此步骤，会形成一个没有内环，没有柄的体
	// 先构造地面的平面
	Vertex* temp_vertex = nullptr;
	Vertex* first_vertex = nullptr;
	Loop* temp_loop = nullptr;
	// TODO:
	// 底面的loop 和顶面的loop
	Loop* first_loop = nullptr;
	Loop* last_loop = nullptr;
	
	glm::vec3 vertex_position;
	
	for (int i = 0; i < this->outline_.vertexes.size(); i++) {
		if (i==0) {
			// 第一个点，构造的第一步
			// 构造体/点/面/环?
			vertex_position = glm::vec3(this->outline_.vertexes[i], 0.0f);
			temp_vertex = this->scene_->eulerOperateMVFS(vertex_position);
			first_vertex = temp_vertex;
			addVertex(temp_vertex);
			first_loop = this->scene_->getSolid(0)->getFace()->getOutLoop();
		}
		else {
			//中间的点，构造边
			// make EV
			// parameter: vertex_1, loop.
			vertex_position = glm::vec3(this->outline_.vertexes[i], 0.0f);
			temp_loop = this->scene_->getNowSolid()->getFace()->getOutLoop();
			temp_vertex = this->scene_->eulerOperateMEV(temp_vertex, temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
			addVertex(temp_vertex);
		}


		// 如果是最后一个点 mef
		if (i==this->outline_.vertexes.size()-1) {
			// 返回的是新loop
			// MEF 
			// input: 两个点，顺序满足h1方向, temp_loop 原来的loop
			// output: 新产生的face的loop
			//std::cout << "Begin last vertex." << std::endl;

			temp_loop = this->scene_->eulerOperateMEF(temp_vertex, first_vertex, temp_loop);
			
			last_loop = temp_loop;
		}
	}
	
	// 构造n个柱子似的边和面

	Vertex* pre = nullptr;
	
	for (int i = 0; i < this->outline_.vertexes.size(); i++) {
		glm::vec3 scale = glm::vec3(this->sweep_scale_);
		vertex_position = glm::vec3(this->outline_.vertexes[i], 0.0f);
		vertex_position += (scale * this->sweep_vector_);
		
		if (i==0) {
			temp_vertex = this->scene_->eulerOperateMEV(getVertex(i), temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
			addVertex(temp_vertex);
			pre = temp_vertex;
		}
		else {
			temp_vertex = this->scene_->eulerOperateMEV(getVertex(i), temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
			addVertex(temp_vertex);
			Vertex* vertex_1 = pre;
			Vertex* vertex_2 = temp_vertex;
			temp_loop = this->scene_->eulerOperateMEF(vertex_1, vertex_2, temp_loop);
			pre = temp_vertex;

		}
	}

	temp_loop = this->scene_->eulerOperateMEF(pre, getVertex((int)this->outline_.vertexes.size()), temp_loop);
	
	last_loop = temp_loop;

	//到此为止，可以构成一个环了吧。。。。
	if (in_ring_.size()<=0) {
		std::cout << "WARRING: There is no ring in solid." << std::endl;
	}

	for (int i = 0; i < in_ring_.size(); i++) {
		/// mev -> mev ->mev -> ... -> mef -> kemr -> sweep

		temp_vertex = first_vertex;
		temp_loop = nullptr;

		Edge* first_edge_in_loop = nullptr;

		for (int j = 0; j < this->in_ring_[i].vertexes.size(); j++) {
			// mev
			vertex_position = glm::vec3(this->in_ring_[i].vertexes[j], 0.0f);
			Edge* temp_edge = this->scene_->eulerOperateMEV(temp_vertex, first_loop, vertex_position);
			temp_vertex = temp_edge->getSecondHalfedge()->getStartVertex();
			addVertex(temp_vertex);
			if (j==0) {
				first_edge_in_loop = temp_edge;
			}

			// 最后一个点 mef
			if (j == in_ring_[i].vertexes.size() - 1) {
				Vertex* temp_vertex_1 = all_vertexes[all_vertexes.size()-in_ring_[i].vertexes.size()];
				Vertex* temp_vertex_2 = temp_vertex;
				
				temp_loop = this->scene_->eulerOperateMEF(temp_vertex_1, temp_vertex_2, first_loop);
				//this->scene_->u
				this->scene_->eulerOperateKEMR(first_edge_in_loop);
			}
		}

		int index = (int)this->all_vertexes.size()-(int)in_ring_[i].vertexes.size();
		
		// sweep in loop
		for (int j = 0; j < this->in_ring_[i].vertexes.size(); j++) {

			glm::vec3 scale = glm::vec3(this->sweep_scale_);
			vertex_position = glm::vec3(this->in_ring_[i].vertexes[j], 0.0f);
			vertex_position += (scale * this->sweep_vector_);

			int temp_index = index + j;
			if (j == 0) {
				temp_vertex = this->scene_->eulerOperateMEV(getVertex(temp_index), temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
				addVertex(temp_vertex);
				pre = temp_vertex;
			}
			else {
				temp_vertex = this->scene_->eulerOperateMEV(getVertex(temp_index), temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
				addVertex(temp_vertex);
				Vertex* vertex_1 = temp_vertex;
				Vertex* vertex_2 = pre;
				temp_loop = this->scene_->eulerOperateMEF(vertex_1, vertex_2, temp_loop);
				pre = temp_vertex;
			}
		}

		temp_loop = this->scene_->eulerOperateMEF(getVertex((int)all_vertexes.size()-(int)this->in_ring_[i].vertexes.size()), pre, temp_loop);

		// KFMRH
		if (temp_loop==nullptr || temp_loop->getFace()==nullptr || last_loop==nullptr||last_loop->getFace()==nullptr) {
			std::cerr << "ERROR: temp_loop->getFace() || last_loop->getFace() is nullptr in Sweeper.cpp_sweep()." << std::endl;
			return;
		}
		else {
			this->scene_->eulerOperateKFMRH(temp_loop->getFace(), last_loop->getFace());
		}
	}

	std::cout << "End Sweep" << std::endl;
	return;
}

// 用于显示生成的图形数据
// 显示每条loop的id normal halfedge position
void Sweeper::showInfo()
{
	std::cout << "============Loop Data Information=============" << std::endl;
	Solid* solid = this->scene_->getSolid(0);

	Face* face = solid->getFace();
	
	while (face!=nullptr) {
		face->showInfo();
		face = face->getNext();
	}

	std::cout << "============Loop Data Information=============" << std::endl;
}



void Sweeper::addVertex(Vertex* vertex)
{
	this->all_vertexes.push_back(vertex);
}

///TODO: comlete myDisplay
void Sweeper::myDisplay()
{

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LINE_SMOOTH);

	// 开启光照
	glLightfv(GL_LIGHT0, GL_POSITION, this->lightPosition_);	//位置
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glEnable(GL_NORMALIZE);

	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
	glClearColor(0,0,0,0);
	glClearDepth(-1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotatef(this->solidRotateAngle_[1], 1.0f, 0.0f, 0.0f);
	glRotatef(this->solidRotateAngle_[0], 0.0f, 1.0f, 0.0f);

	Solid* now_solid = this->scene_->getSolid(0);
	Face* now_face = now_solid->getFace();

	Halfedge* halfedge = nullptr;
	Halfedge* temp_halfedge = nullptr;
	glm::vec3 normal;

	GLUtesselator* tobj = gluNewTess();

	gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK*)())vertexCallbackS);
	gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK*)())beginCallbackS);
	gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK*)())endCallbackS);


	// 现在画所有的面
	std::vector<std::vector<GLdouble>> points(100, std::vector<GLdouble>(3, 0.0f));
	int index = 0;
	int face_cnt = 0;
	//now_face = nullptr;
	while (now_face!=nullptr) {
		face_cnt++;
		// 画一个面
		gluTessBeginPolygon(tobj, nullptr);
		index = 0;
		glColor3f(this->solidColor_[0], this->solidColor_[1], this->solidColor_[2]);
		// 画外轮廓
		normal = now_face->getNormal();
		glNormal3f(normal.x, normal.y, normal.z);
		halfedge = now_face->getOutLoop()->getHalfedge();
		temp_halfedge = halfedge;
		gluTessBeginContour(tobj);
		do {
			points[index][0] = temp_halfedge->getStartVertex()->getPosition().x;
			points[index][1] = temp_halfedge->getStartVertex()->getPosition().y;
			points[index][2] = temp_halfedge->getStartVertex()->getPosition().z;
			gluTessVertex(tobj, (GLdouble*)&(points[index][0]), &(points[index][0]));
			temp_halfedge = temp_halfedge->getNext();
			index++;
		} while (temp_halfedge!=halfedge);
		gluTessEndContour(tobj);
		
		// 画内部的空洞
		Loop* in_loop = now_face->getInLoop();
		while (in_loop!=nullptr) {			
			halfedge = in_loop->getHalfedge();
			temp_halfedge = halfedge;
			gluTessBeginContour(tobj);
			do {
				points[index][0] = temp_halfedge->getStartVertex()->getPosition().x;
				points[index][1] = temp_halfedge->getStartVertex()->getPosition().y;
				points[index][2] = temp_halfedge->getStartVertex()->getPosition().z;
				gluTessVertex(tobj, (GLdouble*)&(points[index][0]), &(points[index][0]));
				temp_halfedge = temp_halfedge->getNext();
				index++;
			} while (temp_halfedge!=halfedge);
			gluTessEndContour(tobj);

			in_loop = in_loop->getNext();
		}
		gluTessEndPolygon(tobj);
		
		now_face = now_face->getNext();
	}
	gluDeleteTess(tobj);

	// 画轮廓
	now_face = now_solid->getFace(0);
	int cnt_vertex = 0;
	
	while (now_face!=nullptr) {
		cnt_vertex = 0;
		halfedge = now_face->getOutLoop()->getHalfedge();
		temp_halfedge = halfedge;
		glColor3f(0.8,0.8,0.8);
		glBegin(GL_LINE_LOOP);
		do {
			cnt_vertex++;
			glm::vec3 pos = temp_halfedge->getStartVertex()->getPosition();
			glVertex3f(pos.x, pos.y, pos.z);
			temp_halfedge = temp_halfedge->getNext();
		} while (temp_halfedge != halfedge);
		glEnd();
		//std::cout << "face id:"<<now_face->getId()<<"vertexes size:" << cnt_vertex << std::endl;
		now_face = now_face->getNext();
	}
	glutSwapBuffers();
}

Vertex* Sweeper::getVertex(int index)
{
	
	if (index>=this->all_vertexes.size()) {
		std::cerr << "ERROR: the index of vertex need [0," << this->all_vertexes.size() << "], but get " << index << " in Sweeper.cpp_getVertex()." << std::endl;
		return nullptr;
	}
	return this->all_vertexes[index];
}

void Sweeper::draw()
{
	std::cout << "Begin Sweeper.draw()." << std::endl;
	
	int argc = 0;
	char** argv = nullptr;
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

	glutInitWindowPosition(100, 100);

	glutInitWindowSize(800, 800);

	glutCreateWindow("Sth's CAD homework.");
	glutKeyboardFunc(Sweeper::keyBoardsCallbackS);
	//this->setupDisplayCallback();

	glutDisplayFunc(Sweeper::displayCallback);
	//glutDisplayFunc(Sweeper::displayCallback);
	
	glutMainLoop();
	
	std::cout << "End Sweeper.draw()." << std::endl;
}