#pragma once

#include "Utilities.h"
#include <stack>
// The skybox only needs access to Utilities for loading cubemaps
// The stack is used for matrix operations

class Skybox
{
private:
	GLuint skybox[6];
	GLuint meshIndexCount;
	GLuint meshObject;
	// A GLuint array and mesh variables declared
	Utilities loader;
	// Utilities object declared
public:
	Skybox();
	void load(); // load method for initilisation
	void draw(stack<glm::mat4> mvStack, GLuint cubemapShaderProgram, mat4 projectionMatrix); // draw method for rendering the Skybox

};
