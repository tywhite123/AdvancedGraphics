#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "Renderer13.h"
#include <time.h>

int main() {

	srand(time(NULL));

	Window w("Cube Mapping! sky textures courtesy of http://www.hazelwhorley.com", 800,600,false);
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer13 renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}