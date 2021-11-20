# Light-Mapping

## Information:
Created using Visual Studio using C++ and GLSL for the rendering aspect and SDL to display.

### Dependencies:
This project requires [GLM](https://glm.g-truc.net/0.9.9/index.html), [GLEW](http://glew.sourceforge.net/) and [SDL](https://www.libsdl.org/download-2.0.php) as dependencies. It is recommended to use [Vcpkg](https://vcpkg.io/en/index.html) to download these dependencies and you will be able to then use the provided CMakeLists file to assemble the dependencies as part of the build process. No linking and fiddling needed!

## Purpose:
This technique demonstrates light mapping on a texture - by sampling the RGB values of an independent texture we can adjust the specular shininess of an object in the scene that appears to have multiple materials. OpenGL was utilised to create the objects seen on screen.

## Link to Demonstration:
[Demo](https://www.youtube.com/watch?v=qNBKvKMv5Ag)
