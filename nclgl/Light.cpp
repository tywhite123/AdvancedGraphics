#include "Light.h"
#include "Window.h"


Light::~Light()
{
}

void Light::UpdateLight(float msec)
{
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_N)) {
		position += Vector3(0,0,10) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_M)) {
		position += Vector3(0, 0, -10) * msec;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_K)) {
		position += Vector3(10, 0, 0) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
		position += Vector3(-10, 0, 0) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Q)) {
		position += Vector3(0, 10, 0) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_E)) {
		position += Vector3(0, -10, 0) * msec;
	}
}
