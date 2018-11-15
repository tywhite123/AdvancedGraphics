#include "Light.h"
#include "Window.h"


Light::~Light()
{
}

void Light::UpdateLight(float msec)
{
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
		position += Vector3(0,0,100) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
		position += Vector3(0, 0, -100) * msec;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
		position += Vector3(100, 0, 0) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
		position += Vector3(-100, 0, 0) * msec;
	}

}
