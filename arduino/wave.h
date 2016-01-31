#ifndef _WAVE_H_
#define	_WAVE_H_

#include "packets.h"
#include "color.h"

color_hsl_t wave_color(packet_wave_t);
//TODO: remove names
float mapf(float x, float in_min, float in_max, float out_min, float out_max);

#endif