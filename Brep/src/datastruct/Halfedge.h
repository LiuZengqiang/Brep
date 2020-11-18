#pragma once
#include"datastruct/AllDataStruct.h"
#include"datastruct/Vertex.h"
#include"datastruct/Edge.h"
#include"datastruct/Loop.h"
class Halfedge
{
public:
	Halfedge(int id, Loop* loop, Halfedge* pre, Halfedge* next, Edge* edge, Vertex* start_vertex, Halfedge* mirror) {
		this->id_ = id;
		this->loop_ = loop;
		this->pre_ = pre;
		this->next_ = next;
		this->edge_ = edge;
		this->start_vertex_ = start_vertex;
		this->mirror_half_edge_ = mirror;
	}
	~Halfedge() {};

	// get and set
	int getId() {
		return this->id_;
	}
	Vertex* getStartVertex() const;
	Halfedge* getPre() const;
	Halfedge* getNext() const;
	Halfedge* getMirrorHalfedge()const;
	Edge* getEdge()const;
	Loop* getLoop()const;

	void setStartVertex(Vertex* startVertex);
	void setPre(Halfedge* preHalfedge);
	void setNext(Halfedge* nextHalfedge);
	void setMirrorHalfedge(Halfedge* mirrorHalfedge);
	void setEdge(Edge* edge);
	void setLoop(Loop* loop);

private:
	int id_;
	Vertex* start_vertex_;
	Halfedge* pre_;
	Halfedge* next_;
	Halfedge* mirror_half_edge_;
	Edge* edge_;
	Loop* loop_;
};