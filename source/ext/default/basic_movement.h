#ifndef BASIC_MOVEMENT_H
#define BASIC_MOVEMENT_H

#include "..\..\engine\render_engine.h"
#include "..\movement.h"

int prime_function(render_engine_struct* re_struct);
int process_function(render_engine_struct* re_struct);
int clean_up_function(render_engine_struct* re_struct);

void set_basic_movement(render_engine_struct* re_struct);

#endif