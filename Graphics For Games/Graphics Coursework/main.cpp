#include "../../nclgl/window.h"
#include "Renderer.h"
#include <time.h>

#pragma comment(lib, "nclgl.lib")

int main() {	
	srand(time(NULL));
	Window w("Graphics Coursework", 1920, 1200, true);
	if(!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	float msec = 0.0f;

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		msec = w.GetTimer()->GetTimedMS();
		renderer.UpdateScene(msec);
		renderer.RenderScene();	
	

	}

	return 0;
}