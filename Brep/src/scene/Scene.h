#pragma once
// ³¡¾°Àà£¬°üº¬solids
#include"datastruct/Solid.h"
#include"glm/glm.hpp"
#include<vector>
class Scene
{
public:
	Scene() {
		this->now_solid_ = nullptr;
		this->solid_cnt = this->face_cnt = this->loop_cnt = this->edge_cnt = this->vertex_cnt = this->halfedge_cnt = 0;
		this->sweep_scale_ = 1.0f;

		this->outline_.clear();
		this->in_ring_.clear();
		this->sweep_vector_ = glm::vec3(0.0f, 0.0f, -1.0f);
		this->all_vertexes.clear();
	};

	~Scene() {};
	
	Solid* getSolid();
	
	/*
	* parameter: new vertex' position
	* return: new vertex
	*/
	Vertex* eulerOperateMVFS(glm::vec3 position);
	/*
	* parameter: first vertex; the loop which contian the new edge; second vertex's position
	* return: new edge
	*/
	Edge* eulerOperateMEV(Vertex* vertex_1, Loop* loop, glm::vec3 position);
	/*
	* parameter: first vertex; second vertex; the loop which contain first and second vertex
	* return: new face's loop
	*/
	Loop* eulerOperateMEF(Vertex* vertex_1, Vertex* vertex_2, Loop* loop);
	/*
	* parameter: edge which will be killed
	* return: new ring(loop)
	*/
	Loop* eulerOperateKEMR(Edge* edge);
	/*
	* parameter: the face's loop which will be kill; the new loop's face
	* return: new loop
	*/
	Loop* eulerOperateKFMRH(Face* face_1, Face* face_2);

	std::vector<glm::vec2>&  getOutline();
	std::vector<glm::vec2>& getInRing(int index);
	int getInRingSize();

	void addInRing(std::vector<glm::vec2>  in_ring);
	glm::vec3 getSweepVector();
	void setSweepVector(glm::vec3 sweep_vector);
	
	float getSweepScale();
	void setSweepScale(float sweep_scale);
	
	Vertex* getVertex(int index);
	void addVertex(Vertex* vertex);
	int getVertexSize();

private:
	
	Solid* now_solid_;	// the solid in the scene(only one solid in one scene).
	
	int solid_cnt;		// solid number, for initialing solid'id
	int face_cnt;		// face number, for initialing face'id
	int loop_cnt;		// ...
	int edge_cnt;		// ...
	int halfedge_cnt;	// ...
	int vertex_cnt;		// ...

	// 
	std::vector<glm::vec2>  outline_;				// the sweeped face's outline
	std::vector<std::vector<glm::vec2> > in_ring_;	// the sweeped face's ring
	glm::vec3 sweep_vector_;						// the sweep vector
	float sweep_scale_;								// the sweep scale
	std::vector<Vertex* > all_vertexes;				// all vertexes of new_solid_;

};