#include"datastruct/Edge.h"
Edge* Edge::getPre()const {
	return this->pre_;
}
Edge* Edge::getNext()const {
	return this->next_;
}
glm::vec3 Edge::getColor() const{
	return this->color_;
}
Halfedge* Edge::getFirstHalfedge()const {
	return this->halfedge_1_;
}
Halfedge* Edge::getSecondHalfedge()const {
	return this->halfedge_2_;
}

void Edge::setPre(Edge* preEdge) {
	this->pre_ = preEdge;
}
void Edge::setNext(Edge* nextEdge) {
	this->next_ = nextEdge;
}
void Edge::setColor(glm::vec3 color) {
	this->color_ = color;
}
void Edge::setColor(float r, float g, float b) {
	this->color_.r = r;
	this->color_.g = g;
	this->color_.b = b;
}
void Edge::setFirstHalfedge(Halfedge* halfedge) {
	this->halfedge_1_ = halfedge;
}
void Edge::setSecondHalfedge(Halfedge* halfedge) {
	this->halfedge_2_ = halfedge;
}
