#ifndef _PACKETS_H_
#define	_PACKETS_H_

#include <stdint.h>

typedef struct {
	uint8_t wave[256];
	uint8_t vol;
	
} packet_wave_t;

typedef struct {
	uint8_t command;
	uint8_t args[8];
	
} packet_control_t;

typedef struct {
	uint8_t R;
	uint8_t G;
	uint8_t B;
	
} packet_set_rgb_t;

typedef struct {
	uint8_t H;
	uint8_t S;
	uint8_t L;
} packet_set_hsl_t;

typedef struct {
	uint8_t mode;
} packet_set_mode_t;

#endif