#include "SceneObjects.h"



SceneObjects::SceneObjects()
{
	//Deafult Constrcutor
}

// This constructor creates an object thats size can be defined through the scale input
// This object only has one texture parameter as no light mapping will be applied to it.
SceneObjects::SceneObjects(vec3 objectPosition, vec3 objectScale, char * textureName)
{
	
	objectPos = objectPosition;
	objectSize = objectScale; //scale of the object can be specified.
	texture = util.loadBitmap(textureName);
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("../assets/cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	material = util.getMaterial();
}

// This constructor creates an object thats size can be defined through the scale input
// It has two textures for light mapping purposes
SceneObjects::SceneObjects(vec3 objectPosition, vec3 objectScale, char * textureName, char * textureName2)
{
	
	objectPos = objectPosition;
	objectSize = objectScale; //scale of the object can be specified.
	texture = util.loadBitmap(textureName);
	texture2 = util.loadBitmap(textureName2);
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("../assets/cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	material = util.getMaterial();
}

// This constructor creates an object thats size can be defined through the scale input
// It can be rotated in the scene
// This object only has one texture parameter as no light mapping will be applied to it.
SceneObjects::SceneObjects(vec3 objectPosition, vec3 objectScale, GLfloat rotation, char * textureName)
{
	
	objectPos = objectPosition;
	objectSize = objectScale; //scale of the object can be specified.
	texture = util.loadBitmap(textureName);
	rotationAmount = rotation;
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("../assets/cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	material = util.getMaterial();
}//Single texture constrcutor

 // This constructor creates an object thats size can be defined through the scale input
 // It can be rotated in the scene
 // It has two texture parameters for light mapping purposes
SceneObjects::SceneObjects(vec3 objectPosition, vec3 objectScale, GLfloat rotation, char * textureName, char * textureName2)
{
	objectPos = objectPosition;
	objectSize = objectScale; //scale of the object can be specified.
	texture = util.loadBitmap(textureName);
	texture2 = util.loadBitmap(textureName2);
	rotationAmount = rotation;
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("../assets/cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	material = util.getMaterial();
}

 // Draws an object that takes a shader
void SceneObjects::draw(stack<glm::mat4> passedStack,  GLuint shaderProgram, mat4 projectionMatrix)
{
	

	// Sets the object into the stack
	stack<glm::mat4> mvStack = passedStack;
	glUseProgram(shaderProgram); //use shader program for shading
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projectionMatrix));
	glBindTexture(GL_TEXTURE_2D, texture);
	mvStack.push(mvStack.top());
	//Moves the cube to its desired position after it has been scaled
	mvStack.top() = glm::translate(mvStack.top(), objectPos);
	//Scales the cube after it is rotated
	mvStack.top() = glm::scale(mvStack.top(), objectSize);
	//Rotates the cube first
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setMaterial(shaderProgram, material);
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();
}

// Draws an object that takes a shader, can draw one or two textures and reacts to a light dynamically.
void SceneObjects::draw(stack<glm::mat4> passedStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, rt3d::lightStruct light)
{
	
	//Sets the object into the stack
	stack<glm::mat4> mvStack = passedStack;
	glUseProgram(shaderProgram); //use shader program for shading
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projectionMatrix));
	if (twoTextures)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Binding for 2 textures
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Binding if only one texture is used
	}
	mvStack.push(mvStack.top());
	//Moves the cube to its desired position after it has been scaled
	mvStack.top() = glm::translate(mvStack.top(), objectPos);
	//Scales the cube after it is rotated
	mvStack.top() = glm::scale(mvStack.top(), objectSize);
	//Rotates the cube first
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setLight(shaderProgram, light);
	rt3d::setMaterial(shaderProgram, material);
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

}

// Draws an object that takes a shader, updates object rotation and reacts to a light dynamically.
void SceneObjects::draw(stack<glm::mat4> passedStack, GLuint shaderProgram, mat4 projectionMatrix, GLfloat rotation, rt3d::lightStruct light)
{
	
	//Sets the object into the stack
	stack<glm::mat4> mvStack = passedStack;
	glUseProgram(shaderProgram); //use shader program for shading
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projectionMatrix));
	glBindTexture(GL_TEXTURE_2D, texture);
	mvStack.push(mvStack.top());
	//Moves the cube to its desired position after it has been scaled
	mvStack.top() = glm::translate(mvStack.top(), objectPos);
	mvStack.top() = glm::rotate(mvStack.top(), float(rotation*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	//Scales the cube after it is rotated
	mvStack.top() = glm::scale(mvStack.top(), objectSize);
	//Rotates the cube first
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setLight(shaderProgram, light);
	rt3d::setMaterial(shaderProgram, material);
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();
}

// Draws an object that takes a shader, updates object rotation , can take one or two textures for drawing
// and reacts to a light dynamically.
void SceneObjects::draw(stack<glm::mat4> passedStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, GLfloat rotation, rt3d::lightStruct light)
{
	
	stack<glm::mat4> mvStack = passedStack;
	glUseProgram(shaderProgram); //use shader program for shading
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projectionMatrix));
	if (twoTextures)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	mvStack.push(mvStack.top());
	//Moves the cube to its desired position after it has been scaled
	mvStack.top() = glm::translate(mvStack.top(), objectPos);
	mvStack.top() = glm::rotate(mvStack.top(), float(rotation*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	//Scales the cube after it is rotated
	mvStack.top() = glm::scale(mvStack.top(), objectSize);
	//Rotates the cube first
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setLight(shaderProgram, light);
	rt3d::setMaterial(shaderProgram, material);
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();
	
}

// Draws an object that takes a shader, updates object rotation , can take one or two textures for drawing,
// update the specular shininess of an object and reacts to a light dynamically.
void SceneObjects::draw(stack<glm::mat4> passedStack, GLuint shaderProgram, mat4 projectionMatrix, bool twoTextures, int textureVisible, int specularValue, GLfloat rotation, rt3d::lightStruct light)
{
	
	stack<glm::mat4> mvStack = passedStack;
	glUseProgram(shaderProgram); //use shader program for shading
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projectionMatrix));
	if (twoTextures)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// For binding two textures
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// For binding one texture
	}
	mvStack.push(mvStack.top());
	//Moves the cube to its desired position after it has been scaled
	mvStack.top() = glm::translate(mvStack.top(), objectPos);
	mvStack.top() = glm::rotate(mvStack.top(), float(rotation*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	//Scales the cube after it is rotated
	mvStack.top() = glm::scale(mvStack.top(), objectSize);
	//Rotates the cube first
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setglUniform1i(shaderProgram, "textureVisible", textureVisible);
	rt3d::setglUniform1i(shaderProgram, "specularValue", specularValue);
	// Passes i_texture_isvisible and texel_specular_value value as uniforms into the shaders to 
	// allow for them to manipulate the scene inside the shader
	rt3d::setLight(shaderProgram, light);
	rt3d::setMaterial(shaderProgram, material);
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();
}

// Texture mutator
void SceneObjects::setTextures(char * textureName, char * textureName2)
{
	texture = util.loadBitmap(textureName);
	texture2 = util.loadBitmap(textureName2);
}

// Updates the scene objects
void SceneObjects::update(vec3 objectPosition, GLfloat rotation)
{
	rotationAmount += rotation;
	objectPos = objectPosition;
}
