#include "Camera.h"

Camera::Camera() :
	quit(false)
{

}
void Camera::Exit()
{
	quit = true;
}