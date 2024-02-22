#ifndef VECTOR_STANDARDS_H
#define VECTOR_STANDARDS_H

#include <float.h>
#include <math.h>

typedef float VECTOR_FLT;
#define VECTOR_FLT_MIN FLT_MIN;
#define VECTOR_FLT_MAX FLT_MAX;

static inline VECTOR_FLT deg_to_rad(VECTOR_FLT deg)
{
	return (deg * (M_PI / 180));
}

#endif