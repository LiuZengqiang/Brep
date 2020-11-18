#pragma once
#include"datastruct/AllDataStruct.h"
#include"datastruct/Halfedge.h"
#include"datastruct/Face.h"

class Loop
{
public:
	Loop(int id, Face* face, Loop* pre, Loop* next, Halfedge* halfedge) {
		this->id_ = id;
		this->face_ = face;
		this->pre_ = pre;
		this->next_ = next;
		this->halfedge_ = halfedge;
	}
	~Loop() {
		
	};

	// get and set
	int getId() {
		return this->id_;
	}
	Halfedge* getHalfedge()const;
	Face* getFace()const;
	Loop* getPre()const;
	Loop* getNext()const;
	int getHalfedgeCount()const;

	void setPre(Loop* preLoop);
	void setNext(Loop* nextLoop);

	void setHalfedge(Halfedge* halfedge);
	void setFace(Face* face);

private:
	int id_;
	Loop* pre_;
	Loop* next_;
	Halfedge* halfedge_;
	Face* face_;
};