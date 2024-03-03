#include "core.h"

int main(void) 
{
	render_engine_struct* re_struct = initialiseRenderEngine();
	if (re_struct == NULL) return 1;

	run(re_struct);
	terminateWindow();
	free(re_struct);
	return 0;
}