#pragma once

#include "Utilities.h"
#include <stack>
// Camera only needs utilities to load shaders and the stack for matrix operations.

using std::stack;
using glm::vec3;

class Camera
{
private:
	//camera properties
	vec3 eye;
	vec3 at;
	vec3 up;
	GLfloat rotation;
	// Varibales for camera declared
	Utilities util; //util object for access to movement methods
public:
	Camera(vec3 eye, vec3 at, vec3 up, GLfloat rotation) :eye(eye), at(at), up(up),rotation(rotation) {};
	//Camera Constructor
	vec3 getEye() { return eye; }
	vec3 getAt() { return at; }
	vec3 getUp() { return up; }
	GLfloat getRotation() { return rotation; }
	void setEye(vec3 e) { eye = e; }
	void setAt(vec3 a) { at = a; }
	void setUp(vec3 u) { up = u; }
	void setRotation(GLfloat rot) { rotation = rot; }
	// Accessors and mutators
	void update(vec3 atPos, GLfloat rotation);
	

};
