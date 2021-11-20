#pragma once

#include "Utilities.h"
#include "Lights.h"
#include <stack>
// Scene objects includes Utilties to load shaders and texutures
// Includes Lights to allow the objects to determine how they will react to a light
// The stack for matrix operations

using glm::mat4;
using glm::vec3;

class SceneObjects
{
private:
	vec3 objectPos;
	// Object position in the scene
	GLuint texture;
	GLuint texture2;
	// Textures for the objects
	int i_texture_isvisible = 0;
	int texel_specular_value = 0;
	// ints to be passed into the shader
	vec3 objectSize;
	GLfloat rotationAmount;
	GLuint meshIndexCount = 0;
	GLuint meshObject;
	Utilities util;
	Lights light;
	rt3d::materialStruct material;
	// Utilities object loaded to access the shaders, the light to allow for setLight commmands
	// To be called and a material is needed as a filler value for the cubes initially.
public:
	//constructor for basic buildings
	SceneObjects();
	//constructor for scalable objects with a bounding volume
	//constructors without rotation
	SceneObjects(vec3 objectPos, vec3 objectScale, char * textureName);
	SceneObjects(vec3 objectPos, vec3 objectScale, char * textureName, char * textureName2);
	SceneObjects(vec3 objectPos, vec3 objectScale, GLfloat rotation, char * textureName);
	SceneObjects(vec3 objectPos, vec3 objectScale, GLfloat rotation, char * textureName, char * textureName2);
	//Constrcutors with rotation
	// There are a number of constructors to allow for the creation of objects with and without double texturing and with or without rotation.
	GLfloat getRotation() { return rotationAmount; }
	void setRotation(GLfloat newRotation) { rotationAmount = newRotation; }
	int getTextureVisible() { return i_texture_isvisible; }
	void setTextureVisible(int textureV) { i_texture_isvisible = textureV; }
	int getSpecularValue() { return texel_specular_value; }
	void setSpecularValue(int specularV) { texel_specular_value = specularV; }
	void setTextures(char * textureName, char * textureName2);
	// Accesors and mutators.
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix);
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, rt3d::lightStruct light);
	// Boxes without rotation
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, GLfloat rotation, rt3d::lightStruct light);
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, GLfloat rotation, rt3d::lightStruct light);
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, int textureVisible, int specularValue, GLfloat rotation, rt3d::lightStruct light);
	//Boxes with rotation
	
	// Updates position and rotation
	void update(vec3 objectPosition, GLfloat rotation);

};