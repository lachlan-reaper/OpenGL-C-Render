#include "core.h"

int main(void) 
{
	GLFWwindow* window = initialiseGLFWWindow();
	if (window == NULL) return 1;

	run(window);
	
	terminateWindow();
	return 0;
}