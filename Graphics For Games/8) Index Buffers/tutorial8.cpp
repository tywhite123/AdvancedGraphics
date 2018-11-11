#include "../../nclgl/window.h"
#include "Renderer5.h"

#pragma comment(lib, "nclgl.lib")

int main() {	
	Window w("Index Buffers!",	1920,1080,false);
	if(!w.HasInitialised()) {
		return -1;
	}

	Renderer6 renderer(w);
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