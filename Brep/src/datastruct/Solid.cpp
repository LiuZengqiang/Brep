#include"datastruct/Solid.h"
Solid* Solid::getPre() const {
	return this->pre_;
}
Solid* Solid::getNext()const {
	return this->next_;
}
Face* Solid::getFace() const{
	return this->face_;
}
Face* Solid::getFace(int index) const
{
	int cnt = 0;
	Face* temp_face = this->face_;
	do {
		if (cnt==index) {
			return temp_face;
		}
		else {
			cnt++;
			temp_face = temp_face->getNext();
		}
	} while (temp_face!=nullptr);
	
	return nullptr;
}
Edge* Solid::getEdge()const {
	return this->edge_;
}
int Solid::getFaceCount() const
{
	int cnt = 0;
	Face* temp_face = this->face_;
	while (temp_face!=nullptr) {
		temp_face = temp_face->getNext();
		cnt++;
	}
	return cnt;

}
void Solid::setPre(Solid* preSolid) {
	this->pre_ = preSolid;
}
void Solid::setNext(Solid* nextSolid) {
	this->next_ = nextSolid;
}
void Solid::setFace(Face* face) {
	this->face_ = face;
}
void Solid::setEdge(Edge* edge) {
	this->edge_ = edge;
}

void Solid::addFace(Face* face)
{
	if (this->face_ == nullptr) {
		this->face_ = face;
	}
	else {
		Face* temp_face = this->face_;
		while (temp_face->getNext()!=nullptr) {
			temp_face = temp_face->getNext();
		}
		temp_face->setNext(face);
		face->setPre(temp_face);
	}
}

void Solid::deleteFace(Face* face)
{
	if (this->face_ == face) {
		this->face_ = face->getNext();
		delete face;
	}
	else {
		face->getPre()->setNext(face->getNext());
		if (face->getNext() != nullptr) {
			face->getNext()->setPre(face->getPre());
			delete face;
		}
	}
}

void Solid::addEdge(Edge* edge)
{
	if (this->edge_ == nullptr) {
		this->edge_ = edge;
	}
	else {
		Edge* temp_edge = this->edge_;
		while (temp_edge->getNext()!=nullptr) {
			temp_edge = temp_edge->getNext();
		}
		temp_edge->setNext(edge);
		edge->setPre(temp_edge);
	}
}


void Solid::deleteEdge(Edge* edge)
{
	if (this->edge_ == edge) {
		this->edge_ = edge->getNext();
		delete edge;
	}
	else {
		edge->getPre()->setNext(edge->getNext());
		if (edge->getNext()!=nullptr) {
			edge->getNext()->setPre(edge->getPre());
			delete edge;
		}
	}
}