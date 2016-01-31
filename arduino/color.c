#include <math.h>
#include "color.h"

#define abs(x) ((x)>0?(x):-(x))

color_rgb_t color_hsl_to_rgb(color_hsl_t col) {
	//H: 0..360; S: 0..1; L: 0..1

	float C = (1 - abs(2*col.L - 1)) * col.S;
	float Hi = col.H/60;

	float X = C * (1 - abs(fmod(Hi, 2) - 1));

	float Ri = 0;
	float Gi = 0;
	float Bi = 0;

	if(Hi < 1) {Ri = C; Gi = X; Bi = 0;} else
		if(Hi < 2) {Ri = X; Gi = C; Bi = 0;} else
			if(Hi < 3) {Ri = 0; Gi = C; Bi = X;} else
				if(Hi < 4) {Ri = 0; Gi = X; Bi = C;} else
					if(Hi < 5) {Ri = X; Gi = 0; Bi = C;} else
						if(Hi < 6) {Ri = C; Gi = 0; Bi = X;}
	float m = col.L - C/2;
	
	color_rgb_t col_rgb = {
		(int)((Ri + m) * 255),
		(int)((Gi + m) * 255),
		(int)((Bi + m) * 255)
	};
	return col_rgb;
}

color_rgb_t color_white_correct(color_rgb_t color, char r, char g, char b) {
	color.R = (int)(floor((float)color.R / 255 * r));
	color.G = (int)(floor((float)color.G / 255 * g));
	color.B = (int)(floor((float)color.B / 255 * b));
	return color;
}