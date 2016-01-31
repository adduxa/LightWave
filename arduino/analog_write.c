#include <avr/io.h>
#include <stdint.h>
#include "analog_write.h"

void digital_write(volatile int *data_port, int pin, int value) {
    if (0 == value) {
        *data_port &= ~(_BV(pin));
    } else {
        *data_port |= _BV(pin);
    }
}

void analog_write(uint8_t val, volatile uint8_t *ocr, volatile uint8_t *port, uint8_t pin, volatile uint8_t *tcr, uint8_t tcr_bit) {
	if (val == 0) {
		/* Turn off PWM */
		*tcr &= ~(_BV(tcr_bit));
		/* Digital write */
		*port &= ~(_BV(pin));
	} else if (val == 255) {
		/* Turn off PWM */
		*tcr &= ~(_BV(tcr_bit));
		/* Digital write */
		*port |= _BV(pin);
	} else {
		/* Turn on PWM */
		*tcr |= _BV(tcr_bit);
		*ocr = val;
	}
}