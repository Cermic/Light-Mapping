 #include "Utilities.h"

// Creates a Utilities object - It has a material as a placeholder to allow other objects
// To instantiate correctly
Utilities::Utilities()
{
	material0 = 
	{
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // ambient
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
		{ 0.0f, 0.1f, 0.0f, 1.0f }, // specular
		2.0f  // shininess
	};
}

// Loads bitmap textures
GLuint Utilities::loadBitmap(char * fname)
	{
		GLuint texID;
		glGenTextures(1, &texID); // generate texture ID

								  // load file - using core SDL library
		SDL_Surface * tmpSurface;
		tmpSurface = SDL_LoadBMP(fname);
		if (!tmpSurface)
		{
			std::cout << "Error loading bitmap" << std::endl;
		}// is tmpSurface is empty an error message is displayed

		 // bind texture and set parameters
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE Replaces GL_REPEAT so that you can't see the edges of the images
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// sets up the texture

		SDL_PixelFormat *format = tmpSurface->format;

		GLuint externalFormat, internalFormat;
		if (format->Amask)
		{
			internalFormat = GL_RGBA;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
		}
		else
		{
			internalFormat = GL_RGB;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0, externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels); // takes in a 2D texture image
		glGenerateMipmap(GL_TEXTURE_2D); //generates a mipmap for the specified texture object

		SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
		return texID;	// return value of texture ID
	}

// Loads 6 textures onto a cubemap
GLuint Utilities::loadCubeMap(const char *fname[6], GLuint *texID)
	{
		glGenTextures(1, texID); // generate texture ID
		GLenum sides[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y };
		SDL_Surface *tmpSurface;

		glBindTexture(GL_TEXTURE_CUBE_MAP, *texID); // bind texture and set parameters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i = 0; i<6; i++)
		{
			// load file - using core SDL library
			tmpSurface = SDL_LoadBMP(fname[i]);
			if (!tmpSurface)
			{
				std::cout << "Error loading bitmap" << std::endl;
				return *texID;
			}

			glTexImage2D(sides[i], 0, GL_RGB, tmpSurface->w, tmpSurface->h, 0,
				GL_BGR, GL_UNSIGNED_BYTE, tmpSurface->pixels);
			// texture loaded, free the temporary buffer
			SDL_FreeSurface(tmpSurface);
		}
		return *texID;	// return value of texure ID, redundant really
	}

// Loads the shaders and projection matrix
void Utilities::loadUtilities()
	{
		projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 1280.0f / 720.0f, 1.0f, 150.0f);
		// Projection setup.

		phongTextureProgram = rt3d::initShaders("../shaders/vertex/phong-tex.vert", "../shaders/fragment/phong-tex.frag");
		rt3d::setMaterial(phongTextureProgram, material0);
		//Phong texture setup

		lightMapProgram = rt3d::initShaders("../shaders/vertex/lightmap.vert", "../shaders/fragment/lightmap.frag");
		//Setting up lightMapProgram

		uniformIndex = glGetUniformLocation(lightMapProgram, "textureUnit1");
		// UniformIndex is the command that passes to the Shaders. //
		glUniform1i(uniformIndex, 1);
		uniformIndex = glGetUniformLocation(lightMapProgram, "textureUnit0");
		glUniform1i(uniformIndex, 0);
		// Lightmap shader setup

		//Shaders loaded for the cubemap
		cubeMapProgram = rt3d::initShaders("../shaders/vertex/cubemap.vert", "../shaders/fragment/cubemap.frag");
		rt3d::setMaterial(cubeMapProgram, material0);
		// Cubemap setup

		shaderProgram = rt3d::initShaders("../shaders/vertex/textured.vert", "../shaders/fragment/textured.frag");
		//shaderProgram setup - uses no lighting or material so that the skybox stays 100% lit.
	}


