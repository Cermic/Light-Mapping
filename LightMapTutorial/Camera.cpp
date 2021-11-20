#include "Camera.h"
		
// Keeps the Camera position and rotation up to date
void Camera::update(vec3 atPos, GLfloat rotation)
{
	at = atPos;
	eye = util.moveForward(at, rotation, 1.0f);
}
