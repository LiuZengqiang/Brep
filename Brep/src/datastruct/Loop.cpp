#include"datastruct/Loop.h"
Halfedge* Loop::getHalfedge()const {
	return this->halfedge_;
}
Face* Loop::getFace()const {
	return this->face_;
}
Loop* Loop::getPre()const {
	return this->pre_;
}
Loop* Loop::getNext()const {
	return this->next_;
}

int Loop::getHalfedgeCount() const
{
	int cnt = 1;
	Halfedge* temp_halfedge = this->halfedge_;
	while (temp_halfedge!=nullptr && temp_halfedge->getNext()!=this->halfedge_) {
		temp_halfedge = temp_halfedge->getNext();
		cnt++;
	}
	return cnt;
}


void Loop::setPre(Loop* preLoop) {
	this->pre_ = preLoop;
}
void Loop::setNext(Loop* nextLoop) {
	this->next_ = nextLoop;
}

void Loop::setHalfedge(Halfedge* halfedge) {
	this->halfedge_ = halfedge;
}
void Loop::setFace(Face* face) {
	this->face_ = face;
}