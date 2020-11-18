#include"datastruct/Halfedge.h"
// get and set
Vertex* Halfedge::getStartVertex()const {
	return this->start_vertex_;
}
Halfedge* Halfedge::getPre()const {
	return this->pre_;
}
Halfedge* Halfedge::getNext()const {
	return this->next_;
}
Halfedge* Halfedge::getMirrorHalfedge() const{
	return this->mirror_half_edge_;
}
Edge* Halfedge::getEdge()const {
	return this->edge_;
}
Loop* Halfedge::getLoop()const {
	return this->loop_;
}

void Halfedge::setStartVertex(Vertex* startVertex) {
	this->start_vertex_ = startVertex;
}
void Halfedge::setPre(Halfedge* preHalfedge) {
	this->pre_ = preHalfedge;
}
void Halfedge::setNext(Halfedge* nextHalfedge) {
	this->next_ = nextHalfedge;
}
void Halfedge::setMirrorHalfedge(Halfedge* mirrorHalfedge) {
	this->mirror_half_edge_ = mirrorHalfedge;
}
void Halfedge::setEdge(Edge* edge) {
	this->edge_ = edge;
}
void Halfedge::setLoop(Loop* loop) {
	this->loop_ = loop;
}