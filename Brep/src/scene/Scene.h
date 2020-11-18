#pragma once
// 场景类，包含solids
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
	* parameter: 新点的坐标
	* return: 新点
	*/
	Vertex* eulerOperateMVFS(glm::vec3 position);
	/*
	* parameter: 第一个vertex，新边所在的loop，新vertex的坐标
	* return: 新edge
	*/
	Edge* eulerOperateMEV(Vertex* vertex_1, Loop* loop, glm::vec3 position);
	/*
	* parameter: 第一个vertex，第二个vertex，第一个和第二个vertex原来所在的loop
	* return: 接下来需要操作的loop，这个loop与halfedge_2相关联，原loop与halfedge_1相关联。
	* 在新建普通的边和建内环时的边时，输入参数vertex_1和vertex_2在实体中的顺序相反。
	*/
	Loop* eulerOperateMEF(Vertex* vertex_1, Vertex* vertex_2, Loop* loop);
	/*
	* parameter: 需要被删除的edge
	* return: 新生成的loop
	*/
	Loop* eulerOperateKEMR(Edge* edge);
	/*
	* parameter: 需要被删除的face，删除face后产生的loop所在的face
	* return: 产生发loop
	*/
	Loop* eulerOperateKFMRH(Face* face_1, Face* face_2);

private:
	std::vector<Solid*> solids_;	// all solids
	Solid* now_solid_;	// 此时正在操作的solid 所有的欧拉变换都将在这个solid上进行 初始时为nullptr
	
	int solid_cnt;
	int face_cnt;
	int loop_cnt;
	int edge_cnt;
	int halfedge_cnt;
	int vertex_cnt;
};