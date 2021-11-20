#include "Skybox.h"

// Creates a skybox
Skybox::Skybox()
{
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices);
	// loads a cube from an external file and applies the norms and vertices along with texture coordinates
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	//creates the mesh for the texture to be applied to with the above variables
}

// Loads the textures onto the cubemap of the skybox
void Skybox::load()
{
	const char *cubeTexFiles[6] =
	{
		"Town-skybox/Town_bk.bmp", "Town-skybox/Town_ft.bmp",
		"Town-skybox/Town_rt.bmp", "Town-skybox/Town_lf.bmp",	
		"Town-skybox/Town_up.bmp", "Town-skybox/Town_dn.bmp"
	};
	loader.loadCubeMap(cubeTexFiles, &skybox[0]);
}

// Draws a cubemapped skybox
void Skybox::draw(stack<glm::mat4> mvStack, GLuint cubemapShaderProgram, mat4 projectionMatrix)
{
	// Drawing the skybox with a cubemap
	glUseProgram(cubemapShaderProgram);
	rt3d::setUniformMatrix4fv(cubemapShaderProgram, "projection", glm::value_ptr(projectionMatrix));
	glDepthMask(GL_FALSE); // make sure writing to update depth test is off - This makes the skybox go to the back of the scene
	glm::mat3 mvRotOnlyMat3 = glm::mat3(mvStack.top());
	mvStack.push(glm::mat4(mvRotOnlyMat3));

	glCullFace(GL_FRONT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox[0]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.5f, 1.5f, 1.5f)); // This will affect how much of the skybox you see in your view frustum
	rt3d::setUniformMatrix4fv(cubemapShaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	glCullFace(GL_BACK); // drawing inside of cube! - the inside is the face we want to see so we cull the outside of the cubes.

	glDepthMask(GL_TRUE); 

						  // Skybox Ends//
}
