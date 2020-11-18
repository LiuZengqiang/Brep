#pragma once
#include"datastruct/AllDataStruct.h"
#include"datastruct/Edge.h"

#include <glm/glm.hpp>
#include <vector>

// Vertex µã
class Vertex {
public:
	Vertex(int id, Vertex* pre, Vertex* next, glm::vec3 position) {
		this->id_ = id;
		this->pre_ = pre;
		this->next_ = next;
		this->position_ = position;
		this->color_ = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	// get and set
	int getId() {
		return this->id_;
	}
	Vertex* getPre() const;
	Vertex* getNext() const;
	glm::vec3 getPosition()const;
	glm::vec3 getColor()const;

	void setPre(Vertex* preVertex);
	void setNext(Vertex* nextVertex);
	void setPosition(glm::vec3 position);
	void setPosition(float x, float y, float z);
	void setColor(glm::vec3 color);
	void setColor(float r, float g, float b);
	

private:
	int id_;
	Vertex* pre_;
	Vertex* next_;
	glm::vec3 position_;
	glm::vec3 color_;
};
