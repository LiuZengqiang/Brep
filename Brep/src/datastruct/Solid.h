#pragma once
//#include"halfEdge.h"
//#include"face.h"
#include"datastruct/AllDataStruct.h"
#include"datastruct/Edge.h"
#include"datastruct/Face.h"

class Solid
{
public:
	Solid(int id, Solid* pre, Solid* next, Face* face, Edge* edge) {
		this->id_ = id;
		this->pre_ = pre;
		this->next_ = next;
		this->face_ = face;
		this->edge_ = edge;
	};
	~Solid() {};
	// get and set
	int getId() {
		return this->id_;
	}

	Solid* getPre()const;
	Solid* getNext()const;
	// return: first face
	Face* getFace()const;
	Face* getFace(int index)const;
	// return: first edge
	Edge* getEdge()const;
	int getFaceCount()const;

	void setPre(Solid* preSolid);
	void setNext(Solid* nextSolid);
	
	void addFace(Face* face);
	void deleteFace(Face* face);

	void addEdge(Edge* edge);
	void deleteEdge(Edge* edge);
private:
	// set first face/edge
	void setFace(Face* face);
	void setEdge(Edge* edge);
	int id_;
	Solid* pre_;
	Solid* next_;
	Face* face_;
	Edge* edge_;
};
