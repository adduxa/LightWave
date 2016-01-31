#include "wave.h"
#include "color.h"
#include "packets.h"
#include "mapf.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

color_hsl_t wave_color(packet_wave_t wave) {
	static color_hsl_t col;
	
	col.H += mapf(wave.vol, 0, 100, 0, 20);
	if(wave.vol > 80 || wave.wave[0] > 30) {
		//col.H += mapf(wave.vol, 100, 200, 40, 90);
	}
	
	float light = constrain(mapf(wave.wave[0], 0, 60, 0.3, 0.6), 0, 1);
	//float light = 0.5;
	if(wave.wave[0] > 20) {
		light += 0.2;
	}
	if(wave.wave[0] > 40) {
		light = 1;
	}

	col.H = col.H + 720 - 360*((col.H+720) / 360);
	col.S = constrain(1, 0, 1);
	col.L = constrain(light, 0, 1);
	return col;
}