#pragma once

#include <GL/glew.h>

#include "Camera.h"
#include "HUD.h"
#include "Lights.h"
#include "Skybox.h"
#include "SceneObjects.h"
#include "Utilities.h"
// Game needs access to any class that will be instantiated by it.

class Game
{
private:
	Camera *camera;
	HUD *hud;
	HUD *keys;
	HUD *nameWaterMark;
	Lights *light;
	Lights *light2;
	Skybox *skybox;
	SceneObjects *box1;
	SceneObjects *box2;
	SceneObjects *lightBox;
	Utilities *util;
	// All of the objects from other classes delcared for use in the scene.

	stack<glm::mat4> mvStack;
	// Stack introduced.
	glm::mat4 modelview;

	int sceneScale;
	// Scene scale declared.
public:
	Game();
	// Game Constructor
	~Game();
	// Game Destructor
	void initialise();
	void render(SDL_Window * window);
	void update();
	// init, render and update functions for the Game class, the Game class itself
	// Is essentially a facade to interface with the main.
	
};