#pragma once
#include"glm/glm.hpp"
#include"datastruct/Halfedge.h"
#include"datastruct/AllDataStruct.h"
#include"datastruct/Solid.h"
class Edge
{
public:
	Edge(int id, Solid* solid, Edge* pre, Edge* next, Halfedge* hafedge_1, Halfedge* halfedge_2)
	{
		this->id_ = id;
		this->solid_ = solid;
		this->pre_ = pre;
		this->next_ = next;
		this->halfedge_1_ = hafedge_1;
		this->halfedge_2_ = halfedge_2;
		this->color_ = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	~Edge() {
	};

	// get and set
	int getId() {
		return this->id_;
	}
	Edge* getPre()const;
	Edge* getNext()const;
	glm::vec3 getColor()const;
	Halfedge* getFirstHalfedge()const;
	Halfedge* getSecondHalfedge()const;

	void setPre(Edge* preEdge);
	void setNext(Edge* nextEdge);
	void setColor(glm::vec3 color);
	void setColor(float r, float g, float b);

	void setFirstHalfedge(Halfedge* halfedge);
	void setSecondHalfedge(Halfedge* halfedge);

private:

	int id_;
	Solid* solid_;
	Edge* pre_;
	Edge* next_;
	glm::vec3 color_;
	Halfedge* halfedge_1_;
	Halfedge* halfedge_2_;
};