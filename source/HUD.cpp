#include "HUD.h"

HUD::HUD()
{
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("../assets/cube.obj", verts, norms, tex_coords, indices);
	// loads a cube from an external file and applies the norms and vertices along with texture coordinates
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	//creates the mesh for the texture to be applied to with the above variables
}

void HUD::load()
{
	// set up TrueType / SDL_ttf
	if (TTF_Init() == -1)
	{
		cout << "TTF failed to initialise." << endl;
	}
	// a catch for failure of initialisation of the Font
	textFont = TTF_OpenFont("../assets/fonts/MavenPro-Regular.ttf", 36); // sets the font to the desired type and font size if the font file has the option
	if (textFont == NULL)
	{
		cout << "Failed to open font." << endl;
	}
	// if the font is null a debug text will be output
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	label[3] = 0;
	label[4] = 0; 
	label[5] = 0;
	label[6] = 0;
	label[7] = 0;
	label[8] = 0;
	label[9] = 0;
	// clears the labels to prevent memory leakage
}

// Takes a character array for the output text, the text ID number and a colour
GLuint HUD::textToTexture(const char * str, GLuint textID, SDL_Color col)

{
	GLuint texture = textID;
	TTF_Font * font = textFont;
	SDL_Color textColour = col;


	SDL_Surface * stringImage = TTF_RenderText_Blended(font, str, textColour);
	// uses a stringImage to render the text with appropriate font, output and colour

	if (stringImage == NULL)
	{
		std::cout << "String surface not created." << std::endl;
	}// if there is a null value in stringImage a debug string is output

	if (texture == 0)
	{
		glGenTextures(1, &texture);
	}// To avoid memory leaks the texutre is only initliased once

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, stringImage->w, stringImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, stringImage->pixels);
	glBindTexture(GL_TEXTURE_2D, NULL);
	// This creates a texture from the string using stingImage width and height along with the BindTexture and TextParemeteri and TextImage2D methods

	SDL_FreeSurface(stringImage); // This frees the RGB surface of the texture
	return texture;
}

// Clears the texture if the textID is empty
void HUD::clearTextTexture(GLuint textID)
{
	if (textID != NULL) {
		glDeleteTextures(1, &textID); // if the textID is empty the texture is cleared
	}
}

// Draws a HUD that displays the keymaps for debugging
void HUD::draw(stack<glm::mat4> mvStack, GLuint shaderProgram)
{
	
	for (int i = 1; i < 9; i++)
	{
		glUseProgram(shaderProgram); //Use texture-only shader for text rendering
		glDisable(GL_DEPTH_TEST); //Disable depth test for HUD label
		glBindTexture(GL_TEXTURE_2D, label[i]); // applies the texture
		mvStack.push(glm::mat4(1.0));
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-0.575f, (-1.1f + (0.2*i)), 0.0f));
		mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.4, 0.1f, 0.0f));
		// moves and scales the texture.
		rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		// resets the projection matrix so that the label will become 2D
		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
		// gets the shader program for drawing so that the label will always be seen

		rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
		mvStack.pop();
		glEnable(GL_DEPTH_TEST);//Re-enable depth test after HUD label
		
	}
		
	
	// The string are all made the same length so that they scale the same when drawn
	string key1 = "Left Shift & Num Pad - Decrease Specular Shininess";
	string key2 = "Left Shift & Num Pad + Increase Specular Shininess";
	string key3 = "Num Key 8 - Loads Lightmap Texture only           ";
	string key4 = "Num Key 7 - Loads Compounds Texture               ";
	string key5 = "Num Key 6 - Loads MetalBox2 Texture Pair          ";
	string key6 = "Num Key 5 - Loads MetalBox Texture Pair           ";
	string key7 = "Num Key 4 - Inverse Rotate Cubes                  ";
	string key8 = "Num Key 3 - Rotate Cubes                          ";

	label[1] = textToTexture(key1.c_str(), label[1], { 255,0,255 });
	label[2] = textToTexture(key2.c_str(), label[2], { 255,0,255 });
	label[3] = textToTexture(key3.c_str(), label[3], { 255,0,255 });
	label[4] = textToTexture(key4.c_str(), label[4], { 255,0,255 });
	label[5] = textToTexture(key5.c_str(), label[5], { 255,0,255 });
	label[6] = textToTexture(key6.c_str(), label[6], { 255,0,255 });
	label[7] = textToTexture(key7.c_str(), label[7], { 255,0,255 });
	label[8] = textToTexture(key8.c_str(), label[8], { 255,0,255 });
}

// Draws a HUD that displays the Specular Shininess value of the scene object.
// Takes the specular value of the object and the shader program along with the modelView stack as parameters.
void HUD::draw(stack<glm::mat4> mvStack, int specularV, GLuint shaderProgram) 
{
	glUseProgram(shaderProgram); //Use texture-only shader for text rendering
	glDisable(GL_DEPTH_TEST); //Disable depth test for HUD label
	glBindTexture(GL_TEXTURE_2D, label[0]); // applies the texture
	mvStack.push(glm::mat4(1.0));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-0.5f, 0.8f, 0.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.5f, 0.2f, 0.0f));
	// moves and scales the texture.
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	// resets the projection matrix so that the label will become 2D
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	// gets the shader program for drawing so that the label will always be seen

	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();
	glEnable(GL_DEPTH_TEST);//Re-enable depth test after HUD label

	string hudOutput = (" Specular Shininess: " + to_string(specularV));

	label[0] = textToTexture(hudOutput.c_str() , label[0], { 255,0,255 });// calls textToTexture giving the string, the label it is to be placed on and the colour
}

// Draws a HUD that displays a specifically defined label on the screen.
// Allows the definition of position, scale and colour from instantiation of the label.
void HUD::draw(stack<glm::mat4> mvStack, GLuint shaderProgram, GLuint labelNumber, vec3 labelPosition, vec3 labelScale, SDL_Color labelColor)
{
	glUseProgram(shaderProgram); //Use texture-only shader for text rendering
	glDisable(GL_DEPTH_TEST); //Disable depth test for HUD label
	glBindTexture(GL_TEXTURE_2D, label[labelNumber]); // applies the texture
	mvStack.push(glm::mat4(1.0));
	mvStack.top() = glm::translate(mvStack.top(), labelPosition);
	mvStack.top() = glm::scale(mvStack.top(), labelScale);
	// moves and scales the texture.
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	// resets the projection matrix so that the label will become 2D
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	// gets the shader program for drawing so that the label will always be seen

	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();
	glEnable(GL_DEPTH_TEST);//Re-enable depth test after HUD label

	string hudOutput = (" Jack D Smith ");

	label[labelNumber] = textToTexture(hudOutput.c_str(), label[labelNumber], labelColor);// calls textToTexture giving the string, the label it is to be placed on and the colour
}

