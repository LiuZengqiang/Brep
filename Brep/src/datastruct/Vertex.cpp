#include"datastruct/Vertex.h"
Vertex* Vertex::getPre() const{
	return this->pre_;
}
Vertex* Vertex::getNext()const {
	return this->next_;
}
glm::vec3 Vertex::getPosition() const{
	return this->position_;
}
glm::vec3 Vertex::getColor() const {
	return this->color_;
}
 
void Vertex::setPre(Vertex* preVertex) {
	this->pre_ = preVertex;
}
void Vertex::setNext(Vertex* nextVertex) {
	this->next_ = nextVertex;
}
void Vertex::setPosition(glm::vec3 position) {
	this->position_ = position;
}
void Vertex::setPosition(float x, float y, float z) {
	this->position_.x = x;
	this->position_.y = y;
	this->position_.z = z;
}
void Vertex::setColor(glm::vec3 color) {
	this->color_ = color;
}
void Vertex::setColor(float r, float g, float b) {
	this->color_.r = r;
	this->color_.g = g;
	this->color_.b = b;
}
 