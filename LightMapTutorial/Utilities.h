#pragma once

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
// Utilities needs access to the rt3d library for creating objects and meshes
// Glm is used for translation operations , rotations and scales

using namespace std;
using glm::mat4;

#define DEG_TO_RADIAN 0.017453293

class Utilities

{
private:
	mat4 projection;
	// declares the projection matrix
	GLuint phongTextureProgram;
	GLuint lightMapProgram;
	GLuint uniformIndex;
	// Creates a uniformIndex to be used for passing data to the shaders.
	GLuint cubeMapProgram;
	GLuint shaderProgram;
	// declares GLuints for the shaders
	rt3d::materialStruct material0;
	// declares the material structure
public:
	Utilities();
	// Deafult Constrcutor for Utilities
	GLuint loadBitmap(char *fname); // method for loading bitmap files into textures
	GLuint loadCubeMap(const char * fname[6], GLuint * texID);
	// Loader methods
	mat4 getProjection() { return projection; }
	void setProjection(mat4 proj) { projection = proj; }
	GLuint getPhongTextureProgram() { return phongTextureProgram;}	
	GLuint getLightMapProgram() { return lightMapProgram; }	
	GLuint getUniformIndex() {return uniformIndex;}	
	GLuint getCubeMapProgram() { return cubeMapProgram; }
	GLuint getShaderProgram() { return shaderProgram; }
	rt3d::materialStruct getMaterial() { return material0; }
	// mutator and accessor methods
	void loadUtilities();
	// methods for accessing the shaders

	glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d)
	{
		return glm::vec3(pos.x + d*std::sin(angle*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(angle*DEG_TO_RADIAN));
	}

	glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d)
	{
		return glm::vec3(pos.x + d*std::cos(angle*DEG_TO_RADIAN), pos.y, pos.z + d*std::sin(angle*DEG_TO_RADIAN));
	}
	// methods to control the player's movement
};