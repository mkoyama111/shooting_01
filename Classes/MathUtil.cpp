#include <assert.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "MathUtil.h"

static float s_sin_table[360];
static float s_cos_table[360];

void MathUtil::Init()
{
    for (int i = 0; i < 360; i++) {
        s_sin_table[i] = sinf((float)M_PI * 2 * i / 360);
    }
    for (int i = 0; i < 360; i++) {
        s_cos_table[i] = cosf((float)M_PI * 2 * i / 360);
    }
}

float MathUtil::sin(unsigned int degrees)
{
    return s_sin_table[degrees % 360];
}

float MathUtil::cos(unsigned int degrees)
{
    return s_cos_table[degrees % 360];
}
