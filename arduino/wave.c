#include <stdio.h>
#include <math.h>
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
	//printf("bass: %d\n", wave.wave[0]);
	float light = constrain(mapf(wave.wave[0], 0, 60, 0.3, 0.6), 0, 1);
	//float light = 0.5;
	if(wave.wave[0] > 20) {
		//light = 1;
		light += 0.2;
		puts("b");
	}
	if(wave.vol > 105) {
		light = 1;
		//light += 0.2;
		puts("L");
	}/*
	if(wave.vol > 70) {
		//light = 1;
		light += 0.2;
		puts("l");
	}*/
	/*if(wave.wave[0] > 40) {
		light = 1;
		puts("l");
	}*/

	//printf("h: %d %d %d %d", wave.vol, (int)mapf(wave.vol, 0, 100, 0, 20), (int)col.H, (int)fmod(col.H, 360.0));
	col.H = fmod(col.H, 360.0);
	col.S = constrain(1, 0, 1);
	col.L = constrain(light, 0, 1);
	return col;
}