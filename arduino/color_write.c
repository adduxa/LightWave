#include <avr/io.h>
#include "color_write.h"
#include "color.h"
#include "analog_write.h"

void color_write(color_rgb_t col) {
	analog_write(col.R, &OCR0B, &PORTD, PORTD5, &TCCR0A, COM0B1);
	analog_write(col.G, &OCR0A, &PORTD, PORTD6, &TCCR0A, COM0A1);
	analog_write(col.B, &OCR2B, &PORTD, PORTD3, &TCCR2A, COM2B1);
}

