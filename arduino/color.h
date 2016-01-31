#ifndef _COLOR_H_
#define	_COLOR_H_

typedef struct {
	float H, S, L;
} color_hsl_t;

typedef struct {
	char R, G, B;
} color_rgb_t;

color_rgb_t color_hsl_to_rgb(color_hsl_t);
color_rgb_t color_white_correct(color_rgb_t, char, char, char);

#endif