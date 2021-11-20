#include "Game.h"

Game::Game()
{

	// Empty Constructor
}

//Destructor to clean up the objects - Game is taking responsibility for scene object destruction
Game::~Game()
{
	delete[] camera;
	delete[] hud;
	delete[] keys;
	delete[] nameWaterMark;
	delete[] light;
	delete[] skybox;
	delete[] box1;
	delete[] box2;
	delete[] lightBox;
	delete[] util;
}

// Initialises and loads all objects in the scene
void Game::initialise()
{
	// Utitlities loaded
	util = new Utilities();
	util->loadUtilities();
	// Camera initialised
	camera = new Camera(vec3(-6.5f, 5.0f, 3.0f),vec3(0.0f, 1.0f, -1.0f),vec3(0.0f, 1.0f, 0.0f),1.0f);
	// HUD initialised
	hud = new HUD();
	hud->load();
	keys = new HUD();
	keys->load();
	nameWaterMark = new HUD();
	nameWaterMark->load();
	// Light initialised
	light = new Lights(vec4(-6.5f, 5.5f, 0.5f, 1.0f), 1.0f);
	//Skybox initialised and loaded.
	skybox = new Skybox();
	skybox->load();

	//initialise openGL to start depth testing and blend
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Sceneobjects loaded
	box1 = new SceneObjects(vec3(-5.0f, 5.0f, -2.5f), vec3(1.0f, 1.0f, 1.0f),1.0f,"../assets/materials/MetalBox.bmp","../assets/materials/MetalBoxLightMapAdvanced.bmp");
	box2 = new SceneObjects(vec3(-8.0f, 5.0f, -2.5f), vec3(1.0f,1.0f,1.0f), 1.0f, "../assets/materials/MetalBox.bmp");
	lightBox = new SceneObjects(light->getLightPos(), vec3(0.25f, 0.25f, 0.25f), "../assets/materials/MetalBox.bmp");
}

// Draws all of the scene objects
void Game::render(SDL_Window * window)
{
	glEnable(GL_CULL_FACE);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection(1.0);// Sets up a fresh projection at the start of the scene
	rt3d::setUniformMatrix4fv(util->getLightMapProgram(), "projection", glm::value_ptr(util->getProjection()));

	GLfloat scale(1.0f); // just to allow easy scaling of complete scene
	glm::mat4 modelview(1.0); // set base position for scene
	mvStack.push(modelview); // modelview is pushed onto the stack to begin

	// Camera set
	camera->setAt(util->moveForward(camera->getEye(), camera->getRotation(), 1.0f));  
	// lookat is updated
	mvStack.top() = mvStack.top() = glm::lookAt(camera->getEye(), camera->getAt(), camera->getUp());
	// Base scene is now setup
	// Skybox drawn
	skybox->draw(mvStack, util->getCubeMapProgram(), util->getProjection());
	// The scene objects decide which shader and light they will use in the scene so it does not matter where the light
	// Is declared in the render portion of the program.
	// Draws the light with lightmapping shader for the lightmapped box
	light->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), light->getAttenuationConstant());
	// Draws a box which is loaded with lightmapping, two textures can be affected by rotation, attenuation and specular shininess changes.
	box1->draw(mvStack, util->getLightMapProgram(), util->getProjection(), true, box1->getTextureVisible(), box1->getSpecularValue(),box1->getRotation() + 0.1f, light->getLight()); // This is the lightmapped box
	// Draws a box which is loaded with regular phong lighting, one texture and can be affected by rotation and attenuation changes.																								
	box2->draw(mvStack, util->getPhongTextureProgram(), util->getProjection(), box2->getRotation() + 0.1f, light->getLight());
	// Draws a box around the light that simply follows it.
	lightBox->draw(mvStack, util->getPhongTextureProgram(), util->getProjection());
	// HUD drawn last so it renders on top.
	hud->draw(mvStack, box1->getSpecularValue(), util->getShaderProgram());
	keys->draw(mvStack, util->getShaderProgram());
	nameWaterMark->draw(mvStack, util->getShaderProgram(), 9, vec3(0.8, 0.9, 0.0), vec3(0.2, 0.1, 0.0), { 255,255,255 });
	mvStack.pop();
	glDepthMask(GL_TRUE);				
	SDL_GL_SwapWindow(window); // swap buffers
}

// Updates all of the scene objects
void Game::update()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	// Updates light attenuation.
	light->update(light->getAttenuationConstant());
	// Updates lightbox position based on the light
	lightBox->update(light->getLightPos(), 1.0f);
	
	// boxes position and rotation updated.
	if (keys[SDL_SCANCODE_3])
	{
		box1->setRotation(box1->getRotation() - 1.0f);
		box2->setRotation(box2->getRotation() + 1.0f);
	}
	// boxes position and rotation updated inversly.
	if (keys[SDL_SCANCODE_4])
	{
		box1->setRotation(box1->getRotation() + 1.0f);
		box2->setRotation(box2->getRotation() - 1.0f);
	}
	// Loads MetalBox texture pair
	if (keys[SDL_SCANCODE_5])
	{
		box1->setTextures("../assets/materials/MetalBox.bmp", "../assets/materials/MetalBoxLightMapAdvanced.bmp");
	}
	// Loads MetalBox2 texture pair
	if (keys[SDL_SCANCODE_6])
	{
		box1->setTextures("../assets/materials/MetalBox2.bmp", "../assets/materials/MetalBox2LightMap.bmp");
	}
	// Shows the full lightmapping effect on the box
	if (keys[SDL_SCANCODE_7])
	{
		box1->setTextureVisible(0);
	}
	// Shows only the box's lightmap texture
	if (keys[SDL_SCANCODE_8])
	{
		box1->setTextureVisible(1);
	}
	
	// Moves camera forward
	if (keys[SDL_SCANCODE_W]) camera->setEye(util->moveForward(camera->getEye(), camera->getRotation(), 0.1f));
	// Moves camera backward
	if (keys[SDL_SCANCODE_S]) camera->setEye(util->moveForward(camera->getEye(), camera->getRotation(), -0.1f));
	// Moves camera left
	if (keys[SDL_SCANCODE_A]) camera->setEye(util->moveRight(camera->getEye(), camera->getRotation(), -0.1f));
	// Moves camera right
	if (keys[SDL_SCANCODE_D]) camera->setEye(util->moveRight(camera->getEye(), camera->getRotation(), 0.1f));
	// Moves camera up
	if (keys[SDL_SCANCODE_UP]) camera->setEye(camera->getEye() + vec3(0.0f,0.1f,0.0f));
	// Moves camera down
	if (keys[SDL_SCANCODE_DOWN]) camera->setEye(camera->getEye() - vec3(0.0f, 0.1f, 0.0f));
	// Rotates camera left
	if (keys[SDL_SCANCODE_LEFT]) camera->setRotation(camera->getRotation() - 1.0f);
	// Rotates right
	if (keys[SDL_SCANCODE_RIGHT]) camera->setRotation(camera->getRotation() + 1.0f);
	// Resets camera
	if (keys[SDL_SCANCODE_R]) camera->setEye(vec3(-6.5f, 5.0f, 3.0f));
	// Camera movement checks

	// Moves light up
	if (keys[SDL_SCANCODE_KP_8])
	{
		light->setLightPos(light->getLightPos() + vec4(0.0f, 0.1f, 0.0f, 0.0f));
	}
	// Moves light down
	if (keys[SDL_SCANCODE_KP_2])
	{
		light->setLightPos(light->getLightPos() - vec4(0.0f, 0.1f, 0.0f, 0.0f));
	}
	// Moves light right
	if (keys[SDL_SCANCODE_KP_6])
	{
		light->setLightPos(light->getLightPos() + vec4(0.1f, 0.0f, 0.0f, 0.0f));
	}
	// Moves light left
	if (keys[SDL_SCANCODE_KP_4])
	{
		light->setLightPos(light->getLightPos() - vec4(0.1f, 0.0f, 0.0f, 0.0f));
	}
	// Moves light forward
	if (keys[SDL_SCANCODE_KP_3])
	{
		light->setLightPos(light->getLightPos() + vec4(0.0f, 0.0f, 0.1f, 0.0f));
	}
	// Moves light backward
	if (keys[SDL_SCANCODE_KP_9])
	{
		light->setLightPos(light->getLightPos() - vec4(0.0f, 0.0f, 0.1f, 0.0f));
	}
	// Draws scene in poly only mode - no textures
	if (keys[SDL_SCANCODE_1]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	// Draws scene normally
	if (keys[SDL_SCANCODE_2]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	// Allows specular changes
	if (keys[SDL_SCANCODE_LSHIFT])
	{
		// Increases specular shininess
		if (keys[SDL_SCANCODE_KP_PLUS]) box1->setSpecularValue(box1->getSpecularValue() + 1);
		// Decreases specular shininess
		if (keys[SDL_SCANCODE_KP_MINUS]) box1->setSpecularValue(box1->getSpecularValue() - 1);
	}
	else
	// Allows attenuation changes
	{
		//Attenutation User Control:
		if (light->getAttenuationConstant() < 1.0f)
		{
			if (keys[SDL_SCANCODE_KP_PLUS]) light->setAttenuationConstant(light->getAttenuationConstant() + 0.01f);
			// Increases attenuation - thus decreasing overall light through restricting it from traveling further unattenuated.
		}
		if (light->getAttenuationConstant() > 0.0f)
		{
			if (keys[SDL_SCANCODE_KP_MINUS]) light->setAttenuationConstant(light->getAttenuationConstant() - 0.01f);
			// Decreases attenuation - thus increasing overall light through allowing it to travel further unattenuated.

		}
	}

}


