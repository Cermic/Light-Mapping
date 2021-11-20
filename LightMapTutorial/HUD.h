#pragma once
#include <string>
#include "Utilities.h" // required shaders
#include <stack> // needs to use a stack
#include <SDL_ttf.h> // needs the extension to use the True type fonts

using namespace std;
using glm::vec3;

class HUD
{
private:
	TTF_Font * textFont; // sets up a pointer to the textFont
	SDL_Color textColour = { 255,255,255 }; // default text colour is white
	GLuint label[10]; // HUD labels
	Utilities loader; // declaring an object of loader
	GLuint meshIndexCount;
	GLuint meshObject;
	// GLuints defined for meshObjects
public:
	HUD();
	void load(); // method for loading the HUD
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram); // draw method that takes the stack, and a shader.
	void draw(stack<glm::mat4> mvStack, int specularV, GLuint shaderProgram); // draw method that takes the stack, the specular value of the light and a shader.
	void draw(stack<glm::mat4> mvStack, GLuint shaderProgram, GLuint labelNumber, vec3 labelPosition, vec3 labelScale, SDL_Color labelColor); // draw method that takes the stack, a shader and the number of the label to be drawn along with a position, scale and colour.
	GLuint textToTexture(const char * str, GLuint textID, SDL_Color col); // method for converting the text to a texture on a mesh
	void clearTextTexture(GLuint textID); // clears texture updates to avoid memeory leak
};
