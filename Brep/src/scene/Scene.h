#pragma once
// �����࣬����solids
#include"datastruct/Solid.h"
#include"datastruct/Face.h"
#include"datastruct/Loop.h"
#include"datastruct/Edge.h"
#include"datastruct/Halfedge.h"
#include"datastruct/Vertex.h"
#include"glm/glm.hpp"
#include<vector>

class Scene
{
public:
	Scene() {
		this->now_solid_ = nullptr;
		this->solid_cnt = this->face_cnt = this->loop_cnt = this->edge_cnt = this->vertex_cnt = this->halfedge_cnt = 0;
	};

	~Scene() {};

	// get and set
	std::vector<Solid*> getAllSolid();
	
	Solid* getSolid(int index);
	Solid* getNowSolid();
	
	void addSolid();
	/*
	* parameter: �µ������
	* return: �µ�
	*/
	Vertex* eulerOperateMVFS(glm::vec3 position);
	/*
	* parameter: ��һ��vertex���±����ڵ�loop����vertex������
	* return: ��edge
	*/
	Edge* eulerOperateMEV(Vertex* vertex_1, Loop* loop, glm::vec3 position);
	/*
	* parameter: ��һ��vertex���ڶ���vertex����һ���͵ڶ���vertexԭ�����ڵ�loop
	* return: ��������Ҫ������loop�����loop��halfedge_2�������ԭloop��halfedge_1�������
	* ���½���ͨ�ıߺͽ��ڻ�ʱ�ı�ʱ���������vertex_1��vertex_2��ʵ���е�˳���෴��
	*/
	Loop* eulerOperateMEF(Vertex* vertex_1, Vertex* vertex_2, Loop* loop);
	/*
	* parameter: ��Ҫ��ɾ����edge
	* return: �����ɵ�loop
	*/
	Loop* eulerOperateKEMR(Edge* edge);
	/*
	* parameter: ��Ҫ��ɾ����face��ɾ��face�������loop���ڵ�face
	* return: ������loop
	*/
	Loop* eulerOperateKFMRH(Face* face_1, Face* face_2);

private:
	std::vector<Solid*> solids_;	// all solids
	Solid* now_solid_;	// ��ʱ���ڲ�����solid ���е�ŷ���任���������solid�Ͻ��� ��ʼʱΪnullptr
	
	int solid_cnt;
	int face_cnt;
	int loop_cnt;
	int edge_cnt;
	int halfedge_cnt;
	int vertex_cnt;
};