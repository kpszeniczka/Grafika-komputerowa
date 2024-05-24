#ifndef PLANE_H
#define PLANE_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Cylinder.h"
#include "OBJLoader.h"

class Plane {
private:
	std::vector<GLfloat> vertices;
public:
	void render();
};
#endif 