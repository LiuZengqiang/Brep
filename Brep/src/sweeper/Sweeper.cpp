#pragma once
// »­³öÀ´Ïß¿ò
#include"Sweeper.h"
#include"json/json.h"
#define NDEBUG
#include"freeglut/freeglut.h"
#include"freeglut/glut.h"
#include<fstream>
#include<iostream>
Sweeper* Sweeper::currentInstance_ = nullptr;

void Sweeper::buildScene(std::string input_file_path){
	std::cout << "Begin buildScene" << std::endl;
	std::ifstream ifs;
	ifs.open(input_file_path);
	if (!ifs.is_open()) {
		std::cerr << "ERROR: Open file:"<<input_file_path<<" fail, please check the file path." << std::endl;
		exit(1);
		return;
	}

	Json::Value root;
	ifs >> root;
	Json::Value face_vertexes = root["Face"]["Vertex"];
	if (face_vertexes.isNull()) {
		std::cerr << "ERROR: File format error in \"Face\"\"Vertex\", please check the file format." << std::endl;
		exit(1);
		return;
	}

	for (unsigned int i = 0; i < face_vertexes.size(); i++) {
		Json::Value temp_value = face_vertexes[i];
		glm::vec2 temp_vec2 = glm::vec2(temp_value[0].asFloat(), temp_value[1].asFloat());

		this->scene_->getOutline().push_back(temp_vec2);
	}
	 
	Json::Value face_ring = root["Face"]["Ring"];
	if (face_ring.isNull()) {
		std::cout << "Info: There is no ring." << std::endl;
	}
	for (unsigned int i = 0; i < face_ring.size(); i++) {
		std::vector<glm::vec2> temp_in_ring_outline;

		for (unsigned int j = 0; j < face_ring[i].size(); j++) {
			Json::Value temp_value = face_ring[i][j];
			glm::vec2 temp_vec2 = glm::vec2(temp_value[0].asFloat(), temp_value[1].asFloat());
			temp_in_ring_outline.push_back(temp_vec2);
		}

		this->scene_->addInRing(temp_in_ring_outline);
	}
	Json::Value sweep_vector = root["SweepVector"];
	if (sweep_vector.isNull()) {
		std::cout << "WARNING: input file format warning, \"SweepVector\" is null, using defalt value." << std::endl;
	}
	else {

		glm::vec3 temp_sweep_vector = glm::vec3(sweep_vector[0].asFloat(), sweep_vector[1].asFloat(), sweep_vector[2].asFloat());

		this->scene_->setSweepVector(temp_sweep_vector);
	}

	Json::Value sweep_scale = root["SweepScale"];
	if (sweep_scale.isNull()) {
		std::cout<<"WARNING: input file format warning, \"SweepScale\" is null, using defalt value." << std::endl;
	}
	else {
		//this->sweep_scale_ = sweep_scale.asFloat();

		this->scene_->setSweepScale(sweep_scale.asFloat());
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
void Sweeper::sweep()
{
	std::cout << "Begin Sweep" << std::endl;
	if (this->scene_ == nullptr) {
		std::cout << "WARRING: Please call initial function first." << std::endl;
		return;
	}
	
	Vertex* temp_vertex = nullptr;
	Vertex* first_vertex = nullptr;
	Loop* temp_loop = nullptr;
	Loop* first_loop = nullptr;
	Loop* last_loop = nullptr;
	
	glm::vec3 vertex_position;
	
	for (int i = 0; i < this->scene_->getOutline().size(); i++) {
		if (i==0) {
			vertex_position = glm::vec3(this->scene_->getOutline()[0], 0.0f);
			temp_vertex = this->scene_->eulerOperateMVFS(vertex_position);
			first_vertex = temp_vertex;
			this->scene_->addVertex(temp_vertex);
			first_loop = this->scene_->getSolid()->getFace()->getOutLoop();
		}
		else {
		
			vertex_position = glm::vec3(this->scene_->getOutline()[i], 0.0f);
			temp_loop = this->scene_->getSolid()->getFace()->getOutLoop();
			temp_vertex = this->scene_->eulerOperateMEV(temp_vertex, temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
			this->scene_->addVertex(temp_vertex);
		}
		if (i==this->scene_->getOutline().size()-1) {
			temp_loop = this->scene_->eulerOperateMEF(temp_vertex, first_vertex, temp_loop);
			
			last_loop = temp_loop;
		}
	}

	Vertex* pre = nullptr;
	for (int i = 0; i < this->scene_->getOutline().size(); i++) {
		glm::vec3 scale = glm::vec3(this->scene_->getSweepScale());
		vertex_position = glm::vec3(this->scene_->getOutline()[i], 0.0f);
		vertex_position += (scale * this->scene_->getSweepVector());
		
		if (i==0) {
			temp_vertex = this->scene_->eulerOperateMEV(this->scene_->getVertex(i), temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
			this->scene_->addVertex(temp_vertex);
			pre = temp_vertex;
		}
		else {
			temp_vertex = this->scene_->eulerOperateMEV(this->scene_->getVertex(i), temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
			this->scene_->addVertex(temp_vertex);
			Vertex* vertex_1 = pre;
			Vertex* vertex_2 = temp_vertex;
			temp_loop = this->scene_->eulerOperateMEF(vertex_1, vertex_2, temp_loop);
			pre = temp_vertex;

		}
	}
	temp_loop = this->scene_->eulerOperateMEF(pre, this->scene_->getVertex((int)this->scene_->getOutline().size()), temp_loop);	
	last_loop = temp_loop;

	if (this->scene_->getInRingSize()<=0) {
		std::cout << "WARRING: There is no ring in solid." << std::endl;
	}

	for (int i = 0; i < this->scene_->getInRingSize(); i++) {
		temp_vertex = first_vertex;
		temp_loop = nullptr;

		Edge* first_edge_in_loop = nullptr;

		for (int j = 0; j < this->scene_->getInRing(i).size(); j++) {
			vertex_position = glm::vec3(this->scene_->getInRing(i)[j], 0.0f);
			Edge* temp_edge = this->scene_->eulerOperateMEV(temp_vertex, first_loop, vertex_position);
			temp_vertex = temp_edge->getSecondHalfedge()->getStartVertex();
			this->scene_->addVertex(temp_vertex);
			if (j==0) {
				first_edge_in_loop = temp_edge;
			}

			if (j == this->scene_->getInRing(i).size() - 1) {
				Vertex* temp_vertex_1 = this->scene_->getVertex(this->scene_->getVertexSize()-this->scene_->getInRing(i).size());
				Vertex* temp_vertex_2 = temp_vertex;
				
				temp_loop = this->scene_->eulerOperateMEF(temp_vertex_1, temp_vertex_2, first_loop);
				this->scene_->eulerOperateKEMR(first_edge_in_loop);
			}
		}

		int index = (int)this->scene_->getVertexSize()-(int)this->scene_->getInRing(i).size();

		for (int j = 0; j < this->scene_->getInRing(i).size(); j++) {

			glm::vec3 scale = glm::vec3(this->scene_->getSweepScale());
			vertex_position = glm::vec3(this->scene_->getInRing(i)[j], 0.0f);
			vertex_position += (scale * this->scene_->getSweepVector());

			int temp_index = index + j;
			if (j == 0) {
				temp_vertex = this->scene_->eulerOperateMEV(this->scene_->getVertex(temp_index), temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
				this->scene_->addVertex(temp_vertex);
				pre = temp_vertex;
			}
			else {
				temp_vertex = this->scene_->eulerOperateMEV(this->scene_->getVertex(temp_index), temp_loop, vertex_position)->getSecondHalfedge()->getStartVertex();
				this->scene_->addVertex(temp_vertex);
				Vertex* vertex_1 = temp_vertex;
				Vertex* vertex_2 = pre;
				temp_loop = this->scene_->eulerOperateMEF(vertex_1, vertex_2, temp_loop);
				pre = temp_vertex;
			}
		}

		temp_loop = this->scene_->eulerOperateMEF(this->scene_->getVertex((int)this->scene_->getVertexSize()-(int)this->scene_->getInRing(i).size()), pre, temp_loop);

		
		this->scene_->eulerOperateKFMRH(temp_loop->getFace(), last_loop->getFace());
	}
	std::cout << std::endl;
	std::cout << "End Sweep" << std::endl;
	return;
}

void Sweeper::showInfo()
{
	std::cout << "============Loop Data Information=============" << std::endl;
	Solid* solid = this->scene_->getSolid();
	
	Face* face = solid->getFace();
	
	while (face!=nullptr) {
		face->showInfo();
		face = face->getNext();
	}

	std::cout << "============Loop Data Information=============" << std::endl;
}

void Sweeper::myDisplay()
{

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LINE_SMOOTH);

	// light
	glLightfv(GL_LIGHT0, GL_POSITION, this->lightPosition_);
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


	Solid* now_solid = this->scene_->getSolid();
	Face* now_face = now_solid->getFace();

	Halfedge* halfedge = nullptr;
	Halfedge* temp_halfedge = nullptr;
	glm::vec3 normal;

	GLUtesselator* tobj = gluNewTess();

	gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK*)())vertexCallbackS);
	gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK*)())beginCallbackS);
	gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK*)())endCallbackS);


	// draw all face
	std::vector<std::vector<GLdouble>> points(100, std::vector<GLdouble>(3, 0.0f));
	int index = 0;
	int face_cnt = 0;

	while (now_face!=nullptr) {
		face_cnt++;
		
		gluTessBeginPolygon(tobj, nullptr);
		index = 0;
		glColor3f(this->solidColor_[0], this->solidColor_[1], this->solidColor_[2]);
		// draw outline
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
		
		//draw hole
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

	now_face = now_solid->getFace(0);
	int cnt_vertex = 0;
	
	// draw edge
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

		now_face = now_face->getNext();
	}
	glutSwapBuffers();
	glutPostRedisplay();
}


void Sweeper::draw()
{
	int argc = 0;
	char** argv = nullptr;
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

	glutInitWindowPosition(100, 100);

	glutInitWindowSize(800, 800);

	glutCreateWindow("12021032's CAD homework.");
	glutKeyboardFunc(Sweeper::keyBoardsCallbackS);

	glutDisplayFunc(Sweeper::displayCallback);
	
	glutMainLoop();
	
}