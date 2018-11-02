#include "../../nclgl/window.h"
#include "Renderer10.h"

#pragma comment(lib, "nclgl.lib")

int main() {	
	Window w("Post Processing!", 1920,1200,true);
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer10 renderer(w);
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