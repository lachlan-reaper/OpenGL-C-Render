#ifndef BASIC_3D_FUNCTIONS_H
#define BASIC_3D_FUNCTIONS_H

#include "..\render_engine.h"
#include "..\objects\camera.h"

int buffer_prime_function(render_engine_struct* re_struct);
int buffer_draw_function(render_engine_struct* re_struct);
int buffer_clean_up_function(render_engine_struct* re_struct);

void set_all_basic_3d_funtions(render_engine_struct* re_struct);

#endif